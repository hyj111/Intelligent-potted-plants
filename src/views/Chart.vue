<template>
  <div class="chart">
    
    <van-cell-group>
  <van-cell title="温度" :value="nowData.temp" />
  <van-cell title="空气湿度" :value="nowData.hum"/>
  <van-cell title="土壤湿度" :value="nowData.soilhum"/>
  <van-cell title="可视化展示">
  <!-- 使用 right-icon 插槽来自定义右侧图标 -->
  <template #right-icon>
    <van-button type="info" size="small" @click="open">查看</van-button>
  </template>
</van-cell>
</van-cell-group>
     <van-overlay :show="show" @click="close">
       <div id="main" style="width: 100vw; height: 400px" @click.stop></div>
      </van-overlay>
  </div>
</template>
<script>
import * as echarts from "echarts";
import { GetAllData } from "../api/getData";
export default {
    props: {
    nowData: {
      type: Object,
      default: () => {
        return {};
      },
    },
  },
  data() {
    return {
      show: false,
      dateList: [],
      tempList: [],
      humList: [],
    };
  },
  methods: {
    drawEchart() {
      // based on prepared DOM, initialize echarts instance
      const myChart = echarts.init(document.getElementById("main"));
      // specify chart configuration item and data
      const option = {
        backgroundColor: '#ffcbd2',
        title: {},
        tooltip: {
          trigger: "axis",
        },
        color: ["#FFA200", "#0ACAE2", "#FF3E43", "#32FF00"], //圆点颜色
        
        legend: {},
        xAxis: {
          data: this.dateList,
          show: false, //隐藏x轴
        },
        yAxis: {
          type: "value",
        },
        series: [
          {
            name: "温度",
            type: "line",
            smooth: 0.3, //波浪
            symbol: "none", //去掉折点形状
            data: this.tempList,
          },
          {
            name: "湿度",
            type: "line",
            smooth: 0.3, //波浪
            symbol: "none", //去掉折点形状
            data: this.humList,
          },
        ],
      };

      // use configuration item and data specified to show chart
      myChart.setOption(option);
    },
    open(){
      this.getData();
      this.show = true;
    },
     close() {
      this.show = false;
    },
    getData() {
      GetAllData("start=2015-01-10T08:00:35").then((res) => {
        let tempArr = res.data.datastreams[0].datapoints;
        let humArr = res.data.datastreams[1].datapoints;
        this.dateList = tempArr.map((item) => {
          return item.at.substring(0,item.at.length-4);
        });
        this.tempList = tempArr.map((item) => {
          return item.value;
        });
        this.humList = humArr.map((item) => {
          return item.value;
        });
        this.drawEchart();
      });
    },
  },
  mounted() {
    // this.getData();
  },
};
</script>

<style scoped>
.chart {
  height: 100vh;
  /* background-color: #ffcbd2; */
}
</style>