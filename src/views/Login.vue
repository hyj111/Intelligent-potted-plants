<template>
  <div class="root">
    <h2 class="title">智能盆栽</h2>
    <svg class="icon" aria-hidden="true" style="font-size: 25px">
            <use xlink:href="#icon-plant" ></use>
    </svg>
    <van-form class="form" @submit="onSubmit">
      <van-tabs v-model="active" @change="tabChange">
        <van-tab title="SIGN IN">
          <van-field
            v-model="form.name"
            label="用户名"
            name="用户名"
            placeholder="用户名"
            :rules="[{ required: true, message: '请填写用户名' }]"
          />
          <van-field
            v-model="form.password"
            type="password"
            label="密码"
            name="密码"
            placeholder="密码"
            :rules="[{ required: true, message: '请填写密码' }]"
          />
          <div style="margin: 16px">
            <van-button round block type="info" native-type="submit"
              >登录</van-button
            >
          </div>
        </van-tab>
        <van-tab title="SIGN UP">
            <van-field
            v-model="form.name"
            label="用户名"
            name="用户名"
            placeholder="用户名"
            :rules="[{ required: true, message: '请填写用户名' }]"
          />
          <van-field
            v-model="form.password"
            type="password"
            label="密码"
            name="密码"
            placeholder="密码"
            :rules="[{ required: true, message: '请填写密码' }]"
          />
           <van-field
            v-model="form.passwordAgain"
            type="password"
            label="确认密码"
            name="确认密码"
            placeholder="确认密码"
            :rules="[{ required: true, message: '请再次输入密码' }]"
          />
          <div style="margin: 16px">
            <van-button round block type="info" native-type="submit" 
              >注册</van-button
            >
          </div>
        </van-tab>
      </van-tabs>
    </van-form>
  </div>
</template>

<script>
import {setToken, setUsername } from "@/utils/cookies";
import { Login,register } from "../api/login";
export default {
  data() {
    return {
      active: 0,
      form: {
        name:'',
        password:'',
        passwordAgain:''
      },
    };
  },
  methods: {
    onSubmit(value){
      this.active===0?this.login():this.register()
    },
    login(){
      Login(this.form).then(res=>{
        setUsername(res.name);
        setToken(res.token);
        this.$router.push('/home')
      }).catch((err) => {
          console.log(err);
        });
    },
    register(){
       register(this.form).then(res=>{
        this.$router.push('/home')
      }).catch((err) => {
          console.log("error");
        });
    },
    tabChange(){
      this.form = {
        name:'',
        password:'',
        passwordAgain:''
      }
    }
  },
};
</script>

<style scoped>
.root {
  position: absolute;
  width: 100vw;
  height: 100vh;
  padding: 0 30px;
  background: #ffcbd2;
}
.title {
  text-align: center;
}
.form {
  background-color: white;
  border-radius: 10px;
  overflow: hidden;
  border-radius: 20px;
  position: absolute;
  top: 50%;
  transform: translateY(-50%);
}
</style>