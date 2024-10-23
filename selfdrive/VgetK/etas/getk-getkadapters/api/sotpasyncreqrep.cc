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
        for (auto sock : poller->poll(1000))
        {
            auto msg = sock->receive();
           api::DataInfoT data_info();
           
            //  BufferInfoT(const BufferAddressT &_address, uint32_t _size, SendDataBufferFlags _flags) : flags(_flags), address(_address), size(_size)
        //    api::BufferInfoT buffer_header(api::SendDataBufferFlags::sync);
           api::BufferInfoT buffer_data(msg->getData(),msg->getSize(),api::SendDataBufferFlags::none);
        //    可以不填充
        //    data_info.buffers.push_back(buffer_header);
           data_info.buffers.push_back(buffer_data);
           data_sender->sendDataBuffer(data_info);
        }
           //返回给Ralo状态更新信息
        ISoTpAsyncResponse* responseHandler;
        responseHandler->updateServiceState();
         return EC_OK;
    }    

}

ErrorCodeT unsubscribe(const types::MappedServiceListT& mpdSrvList) override
{
     for(const auto& mpdSrv:mpdSrvList)
     {
        // 查找服务名称
        ServiceIntT serint = mpdSrv.serviceInt;
        auto server_name = etas_services[int(serint)];
        SubSocket* sock_to_remove = registeredSockets[server_name];
        if (sock_to_remove) {
            poller->unregisterSocket(sock_to_remove);
            // 删除套接字
            delete sock_to_remove;  
        }
     }
       ISoTpAsyncResponse* responseHandler;
       responseHandler->updateServiceState();
       return EC_OK;
}

ErrorCodeT unsubscribeAll() override
{
   for(const auto& Sockets :registeredSockets)
   {
    auto sock = Sockets.second;
    poller->unregisterSocket(sock);
    delete sock;
   }
     ISoTpAsyncResponse* responseHandler;
     responseHandler->updateServiceState();
      return EC_OK;
   
}