<template>
  <div class="home">
    <Plant v-if="index === 0" :nowData="nowData"/>
    <Settings v-if="index === 1" />
    <Chart v-if="index === 2" />
    <van-tabbar v-model="active" @change="onChange">
      <van-tabbar-item icon="home-o">主页</van-tabbar-item>
      <van-tabbar-item icon="flower-o">生长状况</van-tabbar-item>
      <van-tabbar-item icon="setting-o">设置</van-tabbar-item>
    </van-tabbar>
  </div>
</template>

<script>
// @ is an alias to /src
import {GetNowData} from "../api/getData";
import Settings from "./Setting";
import Chart from "./Chart";
import Plant from "./Plant";
export default {
  components: {
    Chart,
    Plant,
    Settings,
  },
  name: "Home",
  data() {
    return {
      active: 0,
      index: 0,
      nowData:{
       temp:0,
        hum:0
      }
      
    };
  },
  methods: {
    onChange(index) {
      this.index = index;
    },
    getData() {
     GetNowData().then(res=>{
       this.nowData.temp = res.data[0].current_value
       this.nowData.hum = res.data[1].current_value
     }).catch(err=>{
       console.log('error');
     })
    },
  },
  mounted() {
    this.getData();
  },
};
</script>
<style scoped>
.home {
}
</style>
