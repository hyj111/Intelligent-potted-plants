<template>
  <div class="chart">
    <div id="main" style="width: 100vw; height: 400px"></div>
  </div>
</template>
<script>
import * as echarts from "echarts";
import { GetAllData } from "../api/getData";
export default {
  data() {
    return {
      dateList: [],
      tempList: [],
      humList: [],
    };
  },
  activated() {
    console.log("床");
  },
  methods: {
    drawEchart() {
      // based on prepared DOM, initialize echarts instance
      const myChart = echarts.init(document.getElementById("main"));
      console.log(this.tempList);
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
    this.getData();
  },
};
</script>

<style scoped>
.chart {
  height: 100vh;
  background-color: #ffcbd2;
}
</style>