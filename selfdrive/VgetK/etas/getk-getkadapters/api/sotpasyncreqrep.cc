#include"sotpasyncreqrep.hpp"
#include
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
        //寻找所有活跃的通道
        types::ServiceDescListT& offer_list;
        types::ServiceDescListT& stop_list;
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
            return EC_ERR_INVALID_HANDLE; // 返回错误代码，因为响应处理器未设置
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
    //服务订阅设置回调流程
}

ErrorCodeT unsubscribe(const types::MappedServiceListT& mpdSrvList) override
{

}

ErrorCodeT unsubscribeAll() override
{

}