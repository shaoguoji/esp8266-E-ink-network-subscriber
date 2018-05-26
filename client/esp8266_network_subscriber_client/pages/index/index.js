//index.js
//获取应用实例
const app = getApp()

Page({
  data: {
    userInfo: {},
    hasUserInfo: false,
    canIUse: wx.canIUse('button.open-type.getUserInfo'),
    activeDevice: app.globalData.activeDevice,
    devices: app.globalData.devices,
    addDeviceToogle: false,
    showTopTips: false,
    num:0,
  },
  //事件处理函数
  bindDeivdeIdInput: function(e) {
    this.data.activeDevice.device_id = e.detail.value;
    this.setData({ activeDevice: this.data.activeDevice});
  },
  bindDeivdeNameInput: function (e) {
    this.data.activeDevice.device_name = e.detail.value;
    this.setData({ activeDevice: this.data.activeDevice});
  },
  addDevice: function() {
    if (this.data.addDeviceToogle == true) {
      console.log('id: ' + this.data.activeDevice.device_id);
      console.log('name: ' + this.data.activeDevice.device_name);
      if (this.data.activeDevice.device_id == '' || this.data.activeDevice.device_name == '') {
        var that = this;
        this.setData({
          showTopTips: true
        });
        setTimeout(function () {
          that.setData({
            showTopTips: false
          });
        }, 3000);
      } else {
        this.data.activeDevice.value = this.data.devices.length;
        this.data.activeDevice.checked = true;
        this.data.activeDevice.device_config = {display_style: 'light', font_size: 1};
        this.setData({ activeDevice: this.data.activeDevice});

        this.data.devices = this.data.devices.concat([this.data.activeDevice]);
        for (var i = 0, len = this.data.devices.length; i < len-1; ++i) {
          this.data.devices[i].checked = false;
        }
        this.setData({ devices: this.data.devices });
        app.globalData.devices = this.data.devices;
        wx.setStorageSync('devices', app.globalData.devices);
        app.globalData.activeDevice = this.data.activeDevice;
        wx.setStorageSync('activeDevice', app.globalData.activeDevice);

        console.log('app.globalData.devices: ' + JSON.stringify(app.globalData.devices));
        console.log('app.globalData.activeDevice' + JSON.stringify(app.globalData.activeDevice))

        this.setData({ addDeviceToogle: false });
        this.data.activeDevice.device_id = '';
        this.data.activeDevice.device_name = '';
        wx.showToast({
          title: '已添加',
          icon: 'success',
          duration: 1000
        });
      }
    } else {
      this.setData({ addDeviceToogle: true });
    }
    
  },
  deviceChange: function (e) {
    console.log('devices发生change事件，携带value值为：', e.detail.value);

    var devices = this.data.devices;
    for (var i = 0, len = devices.length; i < len; ++i) {
      if (devices[i].value == e.detail.value) {
        devices[i].checked = true;
        app.globalData.activeDevice = devices[i];
        wx.setStorageSync('activeDevice', app.globalData.activeDevice);
      } else {
        devices[i].checked = false;
      }

    }
    this.setData({ devices: devices });
    app.globalData.devices = devices;
    wx.setStorageSync('devices', app.globalData.devices);

    console.log('app.globalData.devices: ' + JSON.stringify(app.globalData.devices));
    console.log('app.globalData.activeDevice' + JSON.stringify(app.globalData.activeDevice));
  },
  onLoad: function () {
    if (app.globalData.userInfo) {
      this.setData({
        userInfo: app.globalData.userInfo,
        hasUserInfo: true
      })
    } else if (this.data.canIUse){
      // 由于 getUserInfo 是网络请求，可能会在 Page.onLoad 之后才返回
      // 所以此处加入 callback 以防止这种情况
      app.userInfoReadyCallback = res => {
        this.setData({
          userInfo: res.userInfo,
          hasUserInfo: true
        })
      }
    } else {
      // 在没有 open-type=getUserInfo 版本的兼容处理
      wx.getUserInfo({
        success: res => {
          app.globalData.userInfo = res.userInfo
          this.setData({
            userInfo: res.userInfo,
            hasUserInfo: true
          })
        }
      })
    }
  },
  onShow: function() {
    this.setData({ activeDevice: app.globalData.activeDevice});
    this.setData({ devices: app.globalData.devices});
  },
  getUserInfo: function(e) {
    console.log(e)
    app.globalData.userInfo = e.detail.userInfo
    this.setData({
      userInfo: e.detail.userInfo,
      hasUserInfo: true
    })
  }
})
