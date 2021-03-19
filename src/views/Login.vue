<template>
  <div class="root">
    <h2 class="title">智能盆栽</h2>
    <svg class="icon" aria-hidden="true" style="font-size: 25px">
            <use xlink:href="#icon-plant" ></use>
    </svg>
    <van-form class="form">
      <van-tabs v-model="active" @change="tabChange">
        <van-tab title="SIGN IN">
          <van-field
            v-model="form.username"
            label="名字"
            placeholder="请填写用户名"
            :rules="[{ required: true, message: '请填写用户名' }]"
          />
          <van-field
            v-model="form.password"
            type="password"
            label="密码"
            placeholder="请填写密码"
            :rules="[{ required: true, message: '请填写密码' }]"
          />
          <div style="margin: 16px">
            <van-button round block type="info" native-type="submit" @click="login"
              >登录</van-button
            >
          </div>
        </van-tab>
        <van-tab title="SIGN UP">
            <van-field
            v-model="form.username"
            label="名字"
            placeholder="请填写用户名"
            :rules="[{ required: true, message: '请填写用户名' }]"
          />
          <van-field
            v-model="form.password"
            type="password"
            label="密码"
            placeholder="请填写密码"
            :rules="[{ required: true, message: '请填写密码' }]"
          />
           <van-field
            v-model="form.passwordAgain"
            type="password"
            label="再次输入密码"
            placeholder="请再次输入密码"
            :rules="[{ required: true, message: '请再次输入密码' }]"
          />
          <div style="margin: 16px">
            <van-button round block type="info" native-type="submit" @click="register"
              >注册</van-button
            >
          </div>
        </van-tab>
      </van-tabs>
    </van-form>
  </div>
</template>

<script>
export default {
  data() {
    return {
      active: 0,
      form: {
        username:'',
        password:'',
        passwordAgain:''
      },
    };
  },
  methods: {
    login(){
      this.$router.push('/home') 
      console.log(this.form);
    },
    register(){
      console.log(this.form);
    },
    tabChange(){
      this.form = {
        username:'',
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