<template>
  <div class="home">
    <transition-group>
      <Plant v-if="index === 0" :nowData="nowData" key="Plant" @refresh="refresh"/>
      <Settings v-if="index === 2"  key="Settings"/>
      <Chart v-if="index === 1"  key="Chart" :nowData="nowData"/>
    </transition-group>
    <van-tabbar v-model="active" @change="onChange" class="pink">
      <van-tabbar-item icon="home-o">主页</van-tabbar-item>
      <van-tabbar-item icon="flower-o">生长状况</van-tabbar-item>
      <van-tabbar-item icon="setting-o">设置</van-tabbar-item>
    </van-tabbar>
  </div>
</template>

<script>
// @ is an alias to /src
import { GetNowData } from "../api/getData";
import { GetDevice } from "../api/login";
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
      nowData: {
        temp: 0,
        hum: 0,
      },
    };
  },
  methods: {
    onChange(index) {
      this.index = index;
    },
    getDevice(){
      GetDevice().then(res=>{
        this.$store.commit('setDeviceList',res[0].device)
        this.$store.commit('setDevice',res[0].device[0].device_id)
        this.getData();
      }).catch(err=>{
        console.log(err);
      })
    },
    refresh(){
      this.getDevice()
    },
    getData() {
      GetNowData()
        .then((res) => {
          this.nowData.temp = res.data[0].current_value;
          this.nowData.hum = res.data[1].current_value;
          this.nowData.soilhum = res.data[3].current_value;
        })
        .catch((err) => {
          console.log(err);
        });
    },
  },
  created(){this.getDevice()},
  mounted() {
    
  },
};
</script>
<style scoped>
.v-enter,
.v-leave-to {
  opacity: 0;
}


 .v-enter-active,.v-leave-active{
        transition: all 0.5S ease
    }
</style>
