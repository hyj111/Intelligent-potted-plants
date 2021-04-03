import service from "@/utils/request"
// 获取验证码
export function GetAllData(data){
   return service.request({
        method: "get",
        url: `/datapoints?${data}`,       
    })
}
export function GetNowData(data){
  return service.request({
       method: "get",
       url: `/datastreams`,       
   })
}

export function OpenClose(data) {
    return service.request({
        method: "post",
        url: `/cmds?`,       
    })
}


