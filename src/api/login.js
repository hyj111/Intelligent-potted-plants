import service from "@/utils/request"

export function Login(data){
  return service.request({
       method: "post",
       url: `${process.env.VUE_APP_API_LOGIN}/login`,
       data       
   })
}

export function register(data){
  return service.request({
       method: "post",
       url: `${process.env.VUE_APP_API_LOGIN}/users`,
       data       
   })
}

export function GetDevice(){
  return service.request({
       method: "get",
       url: `${process.env.VUE_APP_API_LOGIN}/device/search`,      
   })
}
// 添加设备
export function addDevice(data){
  return service.request({
       method: "post",
       url: `${process.env.VUE_APP_API_LOGIN}/device`,
       data       
   })
}