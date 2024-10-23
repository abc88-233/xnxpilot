#include"sotpasyncreqrep.hpp"
#include
using namespace etas::getk;
class Impl1:public etas::getk::api::ISoTpAsyncRequest{
    ErrorCodeT find(const types::ServiceStrListT& srvStrList)
    {
        types::ServiceDescListT& offer_list;
        types::ServiceDescListT& stop_list;
        for(const auto& srvStr: srvStrList){
            if(services.find(srvStr)!=services.end())
            {
                return EC_ERR_INVALID_PARAMETER;
            }
            SubMaster sm({srvStr.c_str()});
            auto mes = services[ss];
            if(sm.allAlive)
            {
                offer_list.push_back(types::ServiceDescT(mes.port,ss));
            }
            else{
                stop_list.push_back(types::ServiceDescT(mes.port,ss));
            }
        }
       
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


    ErrorCodeT findAll(){
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

