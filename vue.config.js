module.exports = {
  publicPath: process.env.NODE_ENV === 'production' ? '/supermall/' : './',
  configureWebpack: {
    resolve: {
      alias: {
        'assets': '@/assets',
        'common': '@/common',
        'components': '@/components',
        'network': '@/network',
        'views': '@/views',
        'utils': '@/utils'
      }
    }
  },
  devServer: {
    open: false, // 编译完成是否打开网页
    host: '0.0.0.0', // 指定使用地址，默认localhost,0.0.0.0代表可以被外界访问
    port: 8080, // 访问端口
    https: false, // 编译失败时刷新页面
    hot: true, // 开启热加载
    hotOnly: false,
    proxy: {
      // "/devApi": {
      //   target: "http://api.heclouds.com/devices/694205832", //API服务器的地址
      //   changeOrigin: true,
      //   pathRewrite: {
      //       "^/devApi": ''
      //   }
      // }
      // 需要配置两个跨域地址
      // 这是后端的接口
      [process.env.VUE_APP_API_LOGIN]: {
        target: process.env.VUE_API_DEV_LOGIN_TARGET, //API服务器的地址
        changeOrigin: true,
        pathRewrite: {
          [`^${process.env.VUE_APP_API_LOGIN}`]: ''
        }
      },
      // 这是前端接口
      [process.env.VUE_APP_API_WEB]: {
        target: process.env.VUE_API_DEV_WEB_TARGET, //API服务器的地址
        changeOrigin: true,
        pathRewrite: {
          [`^${process.env.VUE_APP_API_WEB}`]: ''
        }
      }
    }
  }
}