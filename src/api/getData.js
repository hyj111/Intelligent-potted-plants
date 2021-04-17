import service from "@/utils/request"
import store from '../store/index'

export function GetAllData(data){
   return service.request({
        method: "get",
        url: `${process.env.VUE_APP_API_WEB}/${store.state.deviceId}/datapoints?${data}`,       
    })
}
export function GetNowData(data){
  return service.request({
       method: "get",
       url: `${process.env.VUE_APP_API_WEB}/${store.state.deviceId}/datastreams`,       
   })
}
// 新增设备
export function addDevice(data){
    return service.request({
         method: "post",
         url: `${process.env.VUE_APP_API_WEB}`,
         data      
     })
  }

export function OpenClose(data) {
    return service.request({
        method: "post",
        url: `${process.env.VUE_APP_API_WEB}/${store.state.deviceId}/cmds?`,       
    })
}


