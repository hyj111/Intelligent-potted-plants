<template>
  <div class="home">
    <transition-group>
      <Plant v-show="index === 0" :nowData="nowData" key="Plant"/>
      <Settings v-show="index === 2"  key="Settings"/>
      <Chart v-show="index === 1"  key="Chart"/>
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
    getData() {
      GetNowData()
        .then((res) => {
          this.nowData.temp = res.data[0].current_value;
          this.nowData.hum = res.data[1].current_value;
        })
        .catch((err) => {
          console.log("error");
        });
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
.v-enter,
.v-leave-to {
  opacity: 0;
}


 .v-enter-active,.v-leave-active{
        
        transition: all 0.5S ease
    }
</style>
