#include"sotpasyncreqrep.hpp"
#include
using namespace etas::getk;
class Impl1:public etas::getk::api::ISoTpAsyncRequest{
    ErrorCodeT find(const types::ServiceStrListT& srvStrList)
    {
       
      //寻找所有标识为serviceShortIds的服务
        types::ServiceDescListT& offer_list;
        types::ServiceDescListT& stop_list;

        for(const auto& srv : srvStrList){
           //寻找以string标识的通道是否为活跃状态
           auto it = serv.find(srv);
           if(it != serv.end())
           {
            SubSocket* subsock = it->second;
            if(subsock && subsock->alive)
            { //如果为活跃状态，则offer_list添加
                offer_list.push_back();
            }
            else{
                 //如果为不活跃状态，则stop_list添加
               stop_list.push_back();
           }
           }
          else{

          }
   
          return EC
           
        }

        
        //返回给Ralo状态信息
        ISoTpAsyncResponse* responseHandler;
        if(!responseHandler)
        {
            return EC_ERR_INVALID_HANDLE;
        }
        //如果是活跃状态，则offer
        
        responseHandler->offer()
        //如果是不活跃状态，则stopoffer
        responseHandler ->stopOffer();
        
    }
    ErrorCodeT findAll(){
        //寻找所有活跃的通道
             

        //返回给Ralo状态信息
        ISoTpAsyncResponse ->offer();
        ISoTpAsyncResponse ->stopOffer();
    }
}