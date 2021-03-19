<template>
  <div class="plant">
    <img src="../assets/img/giphy.gif" alt="">
    <h4 class="hum">{{nowData.hum}}°C</h4>
    <h4>当前湿度</h4>
    <van-progress :percentage="nowData.temp" stroke-width="15px"/>
    <div class="btn-content">
    <van-button class="btn" type="info" round  @click="watering">点击浇水</van-button>
    <van-button class="btn" type="info" round  @click="bind" color="#07c160">绑定植物</van-button>
    </div>

  </div>
</template>
<script>
export default {
  data() {
    return {
    }
  },
  props:{
    nowData:{
      type:Object,
      default:()=>{
        return{}
      }
    }
  },
   computed: {
    text() {
      return this.currentRate.toFixed(0) + '%';
    },
  },
  methods:{
    watering(){
      this.humidity++
    },
    bind(){}
  }
}
</script>
<style scoped>
.plant {
  background: #ffcbd2;
  height: 100vh;
  text-align: center;
}
h4 {
  margin-bottom: 4px;
}
.btn-content {
  margin-top: 15px;
}
img {
  width: 100%;
}
.hum{
  position: absolute;
  top: 30px;
  left: 50%;
  text-shadow: 5px 5px 5px black, 0px 0px 2px black;
  color: white;
  font-size: 30px;
  transform: translateX(-50%);
}
</style>