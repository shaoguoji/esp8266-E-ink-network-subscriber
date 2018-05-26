//获取应用实例
const app = getApp()

Page({
  data: {
    showTopTips: false,
    styleItems: [
      { name: '浅色主题', value: '0', checked: true },
      { name: '深色主题', value: '1'}
    ],
   fontsizeItems: [
      { name: '小号字体', value: '0', checked: true },
      { name: '中号字体', value: '1' },
      { name: '大号字体', value: '2' }
    ],
    deviceName: '',
    dataSources: [],
    activeDeviceIndex: 0,
  },
  styleChange: function (e) {
    console.log('style发生change事件，携带value值为：', e.detail.value);

    var styleItems = this.data.styleItems;
    for (var i = 0, len = styleItems.length; i < len; ++i) {
      styleItems[i].checked = styleItems[i].value == e.detail.value;
    }

    app.globalData.activeDevice.device_config.display_style = e.detail.value == 0 ? 'light' : 'dark';
    wx.setStorageSync('activeDevice', app.globalData.activeDevice);

    this.setData({
      styleItems: styleItems
    });
  },
  fontsizeChange: function (e) {
    console.log('fontsize发生change事件，携带value值为：', e.detail.value);

    var fontsizeItems = this.data.fontsizeItems;
    for (var i = 0, len = fontsizeItems.length; i < len; ++i) {
      fontsizeItems[i].checked = fontsizeItems[i].value == e.detail.value;
    }

    app.globalData.activeDevice.device_config.font_size = e.detail.value;
    wx.setStorageSync('activeDevice', app.globalData.activeDevice);

    this.setData({
      fontsizeItems: fontsizeItems
    });
  },
  saveConfig: function() {
    // console.log('indexOf: ' + this.data.activeDeviceIndex);
    var index = this.data.activeDeviceIndex;
    app.globalData.devices[index] = app.globalData.activeDevice;

    // wx.setStorageSync('activeDevice', app.globalData.activeDevice);
    wx.setStorageSync('devices', app.globalData.devices);
    wx.showToast({
      title: '已同步',
      icon: 'success',
      duration: 1000
    });
  },
  deleteDevice: function() {
    var index = this.data.activeDeviceIndex;

    wx.showModal({
      title: '提示',
      content: '确认删除设备？',
      confirmText: "确认",
      cancelText: "取消",
      success: function (res) {
        console.log(res);
        if (res.confirm) {
          if (app.globalData.devices.length > 0) {
            app.globalData.devices.splice(index, 1);
            if (app.globalData.devices.length > 0) {
              app.globalData.devices[0].checked = true;
              app.globalData.activeDevice = app.globalData.devices[0];
            } else {
              app.globalData.activeDevice = {};
            }
            wx.setStorageSync('activeDevice', app.globalData.activeDevice);
            wx.setStorageSync('devices', app.globalData.devices);

            console.log('确认删除设备');
            // console.log('app.globalData.devices: ' + JSON.stringify(app.globalData.devices));
            // console.log('app.globalData.activeDevice' + JSON.stringify(app.globalData.activeDevice));
          }
        } else {
          console.log('取消删除设备');
        }
      }
    });

    
  },
  onShow: function() {
    app.globalData.activeDevice = wx.getStorageSync('activeDevice');
    app.globalData.devices = wx.getStorageSync('devices');

    for (var i = 0, len = app.globalData.devices.length; i < len; ++i) {
      if (app.globalData.devices[i].checked == true)
      {
        this.setData({ activeDeviceIndex: i});
        break;
      }
    }

    var style = app.globalData.activeDevice.device_config.display_style;
    var fontSize = app.globalData.activeDevice.device_config.font_size;
    var styleItems = this.data.styleItems;
    var fontsizeItems = this.data.fontsizeItems;
    if (style == 'light') {
      styleItems[0].checked = true;
      styleItems[1].checked = false;
    } else if (style == 'dark') {
      styleItems[0].checked = false;
      styleItems[1].checked = true;
    }

    for (var i = 0, len = fontsizeItems.length; i < len; ++i) {
      fontsizeItems[i].checked = fontsizeItems[i].value == fontSize;
    }

    this.setData({ styleItems: styleItems });
    this.setData({ fontsizeItems: fontsizeItems });
    this.setData({ dataSources: app.globalData.activeDevice.sub_datas });
    this.setData({ deviceName: app.globalData.activeDevice.device_name });
  }
});