import axios from 'axios'
import { Toast } from 'vant';
// import { getToken, getUsername } from "./app";
//创建axios，赋值给service
const BASEURL =  process.env.NODE_ENV === 'production' ? process.env.VUE_APP_API : process.env.VUE_APP_API;
const service = axios.create({
    baseURL:BASEURL,
    timeout: 5000,
});

// 添加请求拦截器
service.interceptors.request.use(function (config) {
    // 在发送请求之前做些什么
    config.headers['api-key'] = 'O=C30hI6pbbe=uZV1p=sxj0TM40=';  // 携带token
    config.headers['Content-Type'] = "application/json";
    return config;
  }, function (error) {
    // 对请求错误做些什么
    return Promise.reject(error);
  });

// 添加响应拦截器
service.interceptors.response.use(function (response) {
    // 对响应数据做点什么
    let data = response.data
    if(response.status ===200){
        return Promise.resolve(data)
    }
   
  }, function (error) {
    // 对响应错误做点什么
    return Promise.reject(error);
  });

export default service