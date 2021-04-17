import Vue from 'vue'
import App from './App.vue'
import router from './router'
import Vant from 'vant';

import store from './store/index';
import './assets/fonts/iconfont.js'
import './assets/fonts/iconfont.css'

import 'vant/lib/index.css';
// import * as echarts from 'echarts'
// Vue.prototype.echarts = echarts
// Vue.use(echarts)
Vue.use(Vant);
Vue.config.productionTip = false

new Vue({
  router,
  store,
  render: h => h(App)
}).$mount('#app')
