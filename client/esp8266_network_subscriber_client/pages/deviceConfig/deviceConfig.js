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
  },
  styleChange: function (e) {
    console.log('style发生change事件，携带value值为：', e.detail.value);

    var styleItems = this.data.styleItems;
    for (var i = 0, len = styleItems.length; i < len; ++i) {
      styleItems[i].checked = styleItems[i].value == e.detail.value;
    }

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

    this.setData({
      fontsizeItems: fontsizeItems
    });
  },
});