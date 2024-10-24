#include"sotpasyncreqrep.hpp"
#include"cereal/messaging/messaging.h"
#include"cereal/service.h"
#include"cereal/etas_service.h"
#include<iostream>


using namespace etas::getk;
class Impl1:public etas::getk::api::ISoTpAsyncRequest{
    ErrorCodeT find(const types::ServiceStrListT& srvStrList) override
    {
        //设置两个ServiceDescListT类型列表，用于向VgetK系统传递活跃与不活跃的通道消息
        types::ServiceDescListT& offer_list;
        types::ServiceDescListT& stop_list;
        //遍历需要查找的服务列表
        for(const auto& srvStr: srvStrList){
            //如果列表不在我们的服务中，则说明参数传递有误
            if(services.find(srvStr)!=services.end())
            {
                return EC_ERR_INVALID_PARAMETER;
            }
            //查找该通道是否活跃
            SubMaster sm({srvStr.c_str()});
            auto mes = services[ss];
            //活跃则放入offer_list中
            if(sm.allAlive)
            {
                offer_list.push_back(types::ServiceDescT(mes.port,ss));
            }
              //不活跃则放入stop_list中
            else{
                stop_list.push_back(types::ServiceDescT(mes.port,ss));
            }
        }
       //定义ISoTpAsyncResponse类用于发送响应
        ISoTpAsyncResponse* responseHandler;
         if (!responseHandler) {
            return EC_ERR_INVALID_HANDLE; // 返回错误代码，因为响应处理器未设置
        }
        if(!offer_list.empty())
        {
            //发送活跃通道
            responseHandler->offer(offer_list);
        }
        if (!stop_list.empty()) {
            //发送不活跃通道
            responseHandler->stopOffer(stop_list);
        }
        return EC_OK;
        
    }


    ErrorCodeT findAll() override
    {
        
        //设置两个ServiceDescListT类型列表，用于向VgetK系统传递活跃与不活跃的通道消息
        types::ServiceDescListT& offer_list;
        types::ServiceDescListT& stop_list;
        //遍历所有通道，寻找所有通道的活跃状态
        for (const auto& it : services) {
          SubMaster sm({(it.first).c_str()});
          if(sm.allAlive)
          {
            offer_list.push_back(types::ServiceDescT(it.second.port,it.first));
          }
          else{
            stop_list.push_back(types::ServiceDescT(it.second.port,it.first));
          }
        }

        //返回给Ralo状态信息
        ISoTpAsyncResponse* responseHandler;
         if (!responseHandler) {
            // 返回错误代码，因为响应处理器未设置
            return EC_ERR_INVALID_HANDLE; 
        }
        if(!offer_list.empty())
        {
            responseHandler->offer(offer_list);
        }
        if (!stop_list.empty()) {
            responseHandler->stopOffer(stop_list);
        }
        return EC_OK;
    }


ErrorCodeT subscribe(const types::MappedServiceListT& mpdSrvList) override
{
    //user should parse the parameter mappedServices and do subscription
    // 1.get the shortId from MappedServiceListT.serviceInt and find the service object
    //寻找shortID
    // 2.get the contextId from MappedServiceListT.contextId
    Context* ctx = Context::create();
    for(const auto& mpdSrv:mpdSrvList)
    {   //获取serviceint
        ServiceIntT serint = mpdSrv.serviceInt;
        //获取contextid
        ContextIdT con_id = mpdSrv.contextId;
        //寻找服务名称
        auto server_name = etas_services[int(serint)];
      //服务订阅
        SubSocket* sock = SubSocket::create(ctx, server_name.c_str());
        poller->registerSocket(sock);
        registeredSockets[server_name]=sock;
    }
      // 3.do subscription on the service object and with a callback which eventually call IDataSender->sendDataBuffer
    while(true)
    {
            //服务订阅设置回调流程
        IDataSender* data_sender;
        types::ServiceDescExtListT& srvDescExtList;
        for (auto sock : poller->poll(1000))
        {
            auto msg = sock->receive();
            auto msg_data = msg->getData();
            uint32_t msg_size = msg->getSize();
// DataInfoT(types::ContentTypeT _type, types::ContextIdT _contextId,types::ServiceIntT _ecuEventId,
// types::TimestampT _timestamp,DataInfoHandleT _handle, EventPriorityT _priority) 
            auto time_stamp = std::chrono::steady_clock::now();
            api::DataInfoT data_info(types::ContentTypeT::DATA，con_id,serint,time_stamp,0，0);
            void* voidPtr = const_cast<void*>(msg_data);
            api::BufferAddressT buf_adr(voidPtr, 0);
            //  BufferInfoT(const BufferAddressT &_address, uint32_t _size, SendDataBufferFlags _flags) : flags(_flags), address(_address), size(_size)
            // api::BufferInfoT buffer_header(api::SendDataBufferFlags::sync);
            api::BufferInfoT buffer_data(msg->getData(),msg_size,api::SendDataBufferFlags::none);
        //    可以不填充
            // data_info.buffers.push_back(buffer_header);
            data_info.buffers.push_back(buffer_data);
            data_sender->sendDataBuffer(data_info);
            auto serdes = types::ServiceDescT(serint,server_name);
            srvDescExt.serviceDesc=serdes;
            srvDescExt.serviceState=types::ServiceStateT::SUBSCRIBED;
            srvDescExtList.push_back(srvDescExt);
        }
           //返回给Ralo状态更新信息
        ISoTpAsyncResponse* responseHandler;
        responseHandler->updateServiceState(srvDescExtList);
         return EC_OK;
    }    

}

ErrorCodeT unsubscribe(const types::MappedServiceListT& mpdSrvList) override
{
     types::ServiceDescExtListT& srvDescExtList;
     for(const auto& mpdSrv:mpdSrvList)
     {
        // 查找服务名称
        ServiceIntT serint = mpdSrv.serviceInt;
        auto server_name = etas_services[int(serint)];
        SubSocket* sock_to_remove = registeredSockets[server_name];
        types::ServiceDescExtT& srvDescExt
        auto serdes = types::ServiceDescT(serint,server_name);
        srvDescExt.serviceDesc=serdes;
        srvDescExt.serviceState=types::ServiceStateT::UNDEFINED;
        srvDescExtList.push_back(srvDescExt);
        if (sock_to_remove) {
            poller->unregisterSocket(sock_to_remove);
            // 删除套接字
            delete sock_to_remove;  
        }
     }
       ISoTpAsyncResponse* responseHandler;
       responseHandler->updateServiceState(srvDescExtList);
       return EC_OK;
}

ErrorCodeT unsubscribeAll() override
{
   types::ServiceDescExtListT& srvDescExtList;

   for(const auto& Sockets :registeredSockets)
   {
    auto sock = Sockets.second;
    poller->unregisterSocket(sock);
    types::ServiceDescExtT& srvDescExt
    auto name = Sockets.first;
    auto srvint = services[name];
    auto serdes = types::ServiceDescT(srvint,name);
    srvDescExt.serviceDesc=serdes;
    srvDescExt.serviceState=types::ServiceStateT::UNDEFINED;
    srvDescExtList.push_back(srvDescExt);
    delete sock;
    
   }
     ISoTpAsyncResponse* responseHandler;
     responseHandler->updateServiceState(srvDescExtList);
    return EC_OK;
}


ErrorCodeT requestContent(const types::ContextIdT contextId, const types::ServiceIntListT& srvIntList, const types::ContentTypeT type) override
{
    ISoTpAsyncResponse* responseHandler;
    IDataSender* data_sender;
    auto localwriter = [this](types::ContextIdT contextId , types::ServiceIntT shortId,const void* const data, uint32_t datasize,types::ContentTypeT type)
    {
        api::BufferAddressT addr{data,0};
        api::BufferInfoT buffer_info{addr,dataSize, api::SendDataBufferFlags::sync};
        const auto rx_timestamp = std::chrono::steady_clock::now();
        uint64_t timestamp_ns = static_cast<std::uint64_t>( std::chrono::duration_cast<std::chrono::nanoseconds>(rx_timestamp.time_since_epoch()).count());
        types::DataInfoT datainfo(type,contextId,shortId,{timestamp_ns},0,0);
        datainfo.buffers.push_back(buffer_info);
        data_sender->sendDataBuffer(datainfo);
    };
    switch(type)
    {
        case types::ContentTypeT::GLOBAL_HEADER:
        {
             types::ServiceIntT shortId =0xffffffff;
             std::string header = 'BYD V1.0';
             const void* header_addr = header.data();
             auto header_size = header.size();
             localWrite(contextId,shortId,header_addr,header_size,etas::getk::types::ContentTypeT::GLOBAL_HEADER);
             responseHandler->requestContentCompleted(contextId);
             break;
        }
        case types::ContentTypeT::GLOBAL_STATIC_INFO:
        {
            // for(const auto& serviceInfo : this->serviceTopicManager_.getServiceInfoList(stvIntList))
            // {
            //     auto schema = xxx;
            //     localWrite(contextId,serviceInfo.shortId,schema->data(),schema->size(),etas::getk::types::ContentTypeT::METADATA);
            // }
            responseHandler->requestContentCompleted(contextId);
            break;
        }
        case types::ContentTypeT::GLOBAL_DYNAMIC_INFO:
        {
            //send global dynamic data
            responseHandler->requestContentCompleted(contextId);
            break;
        }
        case types::ContentTypeT::METADATA:
        {
            // for(const auto& serviceInfo : this->serviceTopicManager_.getServiceInfoList(stvIntList))
            // {
            //     auto channel = xxx;
            //     localWrite(contextId,serviceInfo.shortId,channel->data(),channel->size(),etas::getk::types::ContentTypeT::METADATA);
            // }
            responseHandler->requestContentCompleted(contextId);
            break;
        }
        case types::ContentTypeT::DATA:
        {
            //  for(const auto& serviceInfo : this->serviceTopicManager_.getServiceInfoList(stvIntList))
            // {
            //     auto buffer_info = xxx;
            //     types::DataInfoT datainfo(type,contextId,shortId,{timestamp_ns},0,0);
            //     datainfo.buffers.push_back(buffer_info);
            //     data_sender->sendDataBuffer(datainfo);
            // }
            responseHandler->requestContentCompleted(contextId);
            break;
        }
        default:
            responseHandler->requestContentCompleted(contextId);
            break;
    }
    return EC_OK;
}
 }