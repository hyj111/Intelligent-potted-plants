import axios from 'axios'
import {
  getToken,
  getUsername
} from "./cookies";
//创建axios，赋值给service

const service = axios.create({
  baseURL: '',
  timeout: 5000,
});

// 添加请求拦截器
service.interceptors.request.use(function (config) {
  // 在发送请求之前做些什么
  if (config.url.indexOf('apiLogin') !== -1) {
    config.headers['Authorization'] = getToken();// 携带token
  } else if (config.url.indexOf('apiWeb') !== -1) {
    config.headers['api-key'] = 'O=C30hI6pbbe=uZV1p=sxj0TM40='; 
    config.headers['Content-Type'] = "application/json";
  }


  return config;
}, function (error) {
  // 对请求错误做些什么
  return Promise.reject(error);
});

// 添加响应拦截器
service.interceptors.response.use(function (response) {
  // 对响应数据做点什么
  let data = response.data
  if (response.status === 200) {
    return Promise.resolve(data)
  } else {
    return data
  }
}, function (error) {
  // 对响应错误做点什么
  return Promise.reject(error);
});

export default service