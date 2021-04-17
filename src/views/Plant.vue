<template>
  <div class="plant">
    <van-swipe @change="onChange">
      <van-swipe-item
        v-for="(item, index) in $store.state.deviceList":key="index">
        <img src="../assets/img/giphy.gif" alt="" />
        <h4 class="hum">{{ nowData.hum }}</h4>
        <h4>{{item.title}}</h4>
        <h4 class="temp">{{ nowData.temp }}°C</h4>
      </van-swipe-item>
      <template #indicator>
        <div class="custom-indicator">{{ current + 1 }}/{{$store.state.deviceList.length}}</div>
      </template>
    </van-swipe>
    <div class="btn-content">
      <van-button class="btn" type="info" round @click="watering"
        >点击浇水</van-button
      >
      <van-button class="btn" type="info" round @click="open" color="#07c160"
        >绑定植物</van-button
      >
      <van-overlay :show="show" @click="close">
        <div class="wrapper" @click.stop>
          <van-form @submit="onSubmit">
            <van-field
              v-model="form.title"
              label="设备名"
              name="title"
              placeholder="设备名"
              :rules="[{ required: true, message: '请填写设备名' }]"
            />
            <van-field
              v-model="form.deviceId"
              label="设备ID"
              name="deviceId"
              placeholder="设备ID"
              :rules="[{ required: true, message: '请填写设备ID' }]"
            />
            <van-button block type="info" native-type="submit">绑定</van-button>
          </van-form>
        </div>
      </van-overlay>
    </div>
  </div>
</template>
<script>
import { Toast } from "vant";
import { addDevice } from "../api/login";
export default {
  data() {
    return {
      show: false,
      current: 0,
      form: {
        title: "",
        deviceId: "",
      },
    };
  },
  props: {
    nowData: {
      type: Object,
      default: () => {
        return {};
      },
    },
  },
  computed: {
    text() {
      return this.currentRate.toFixed(0) + "%";
    },
  },
  methods: {
    watering() {
      this.humidity++;
    },
    onChange(index) {
       this.current = index;
       let info = this.$store.state.deviceList[index]
       this.$store.commit('setDevice',info.device_id)
       console.log(this.$store.state.deviceId);
    },
    onSubmit(value) {
      addDevice(this.form)
        .then((res) => {
          Toast.success("绑定成功");
          this.$emit("refresh");
          this.show = false;
        })
        .catch((err) => {
          Toast.fail("设备已被绑定");
        });
    },
    bind() {},
    open() {
      this.show = true;
    },
    close() {
      this.show = false;
      this.form = {
        title: "",
        deviceId: "",
      };
    },
  },
};
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
.hum {
  position: absolute;
  top: 28px;
  left: 20%;
  text-shadow: 5px 5px 5px black, 0px 0px 2px black;
  color: white;
  font-size: 30px;
  /* transform: translateX(-50%); */
}
.temp {
  position: absolute;
  top: 28px;
  right: 20%;
  text-shadow: 5px 5px 5px black, 0px 0px 2px black;
  color: white;
  font-size: 30px;
}
  .custom-indicator {
    position: absolute;
    right: 5px;
    bottom: 5px;
    padding: 2px 5px;
    font-size: 12px;
    background: rgba(0, 0, 0, 0.1);
  }
</style>