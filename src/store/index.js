import Vue from 'vue'
import Vuex from 'vuex'
// 1.安装插件
Vue.use(Vuex)
// 2.创建对象
const store = new Vuex.Store({
  state: {
    deviceId: '',
    deviceList:'',
  },
  mutations: {
    setDevice(state, deviceId) {
      state.deviceId = deviceId; //将传参设置给state的city
    },
    setDeviceList(state,deviceList){
      state.deviceList = deviceList;
    }
  },
  actions: {},
  getters: {},
  modules: {},
})
// 3.导出store共享
export default store