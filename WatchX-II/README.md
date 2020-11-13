# WatchX-II 更新日志
> https://github.com/FASTSHIFT/WatchX-II

## [v0.1] - 2020-08-31
* 1.框架搭建完成，UI基于LVGL v7.4.0，跑分120FPS+
* 2.硬件I2C调试通过，所有设备读写正常
* 3.添加RTC库
* 4.添加AT32F403A文档

## [v0.2] - 2020-09-02
* 1.添加LVGL PDF文档
* 2.添加字体转换工具：LvglFontTool，图片转换工具：Lvgl_image_convert_tool
* 3.添加第一个表盘页面
* 4.添加新字体

## [v0.3] - 2020-09-07
* 1.表盘设计完成
* 2.修改mono主题
* 3.添加新的字体RexBold
* 4.添加label滑动效果库
* 5.添加页面注册宏

## [v0.4] - 2020-09-22
 * 1.调整DISP刷新周期至20ms
 * 2.修改PageManger的event类型为uint8_t，与lv_obj_event_cb兼容
 * 3.更优雅的页面注册宏，使用PAGE_EXPORT和PAGE_IMPORT
 * 4.使用lvgl提供的gestute功能，屏幕的gestute事件会传递给页面事件
 * 5.独立DisplayPage.cpp
 * 6.加入微软雅黑字体
 * 7.添加计算器页面
 * 8.添加主菜单，精致优雅的非线性动画
 * 9.使用宏简化表盘页面繁琐的动画函数
 * 10.添加lv_port_log日志输出接口
 * 11.添加专属主题lv_theme_watchx
 * 12.添加LVGL扩展功能文件组lv_ext
 
## [v0.5] - 2020-10-04
 * 1.修复ADC问题
 * 2.移植FreeRTOS(V10.4.1)
 * 3.LVGL使用FreeRTOS提供的内存管理
 * 4.LVGL使用标准的memset和memcpy
 * 5.添加lv_sim_visual_studio_sdl模拟器，可直接在PC上调试UI
 * 6.添加按键支持
 * 7.添加Power管理
 * 8.移植X-CTRL的线性马达驱动
 * 9.分离FT6336U驱动至BSP TouchPad
 * 10.修改计步数上限
 * 11.UI层使用lv_tick_get()，减少millis()依赖
 * 12.添加秒表页面(由@Trigger-CN设计)
 * 13.修复主菜单滑动动画BUG

## [v0.6] - 2020-11-03
* 1.添加ADC_Reset
* 2.AT32F4XX标准外设库升级至 v1.3.4
* 3.改进硬件i2c库
* 4.为cm_backtrace添加FreeRTOS支持
* 5.LVGL使用自带的内存管理和内存区域，减小FreeRTOS的HEAP至30KB
* 6.更新LVGL库至v7.7.1
* 7.更新lv_conf.h新添加的内容
* 8.添加ComMaster-Slave数据透传库，支持使用外部MCU读取传感器数据返回给主机，多数据节点支持
* 9.添加ParticleSensor_GetBeats()
* 10.使用DMA方式读取电池电压
* 11.添加BSP/Built-in可以在内置I2C/外置MCU I2C两种传感器数据获取方式之间切换，使用宏开关：WX_USE_BUILT_IN_I2C
* 12.去除AppWindow_GetHeight/Width函数
* 13.添加可隐藏式状态栏StatusBar，位于lv_layer_sys()图层
* 14.添加HeartRate心率显示页面(由@Trigger-CN设计)
* 15.添加Settings系统设置页面(由@Trigger-CN设计)
* 16.添加TimeSet时间设置页面(由@Trigger-CN设计)
* 17.添加BacklightSet背光设置页面(由@Trigger-CN设计)
* 18.改进PageDelay，不再关心时间溢出问题
* 19.添加MicrosoftYahei_16字体
* 20.扩充MicrosoftYahei_28字体的符号
* 21.添加lv_anim_timeline库，实现复杂的动画组合
* 22.使用lv_anim_path_ease_out替代lv_anim_path_strong_ease_in_out
* 23.为lv_obj_add_anim添加动画启动延时
* 24.改进DialPlate表盘页面的动画
* 25.改进Stopwatch秒表页面的动画
* 26.更新PageManager v1.5

## [v0.7] - 2020-11-13
* 1.添加I2C Slave (STM32F1)
* 2.添加Arduboy/Arduboy2 Game System
* 3.添加ArduboyTones
* 4.添加游戏《Arduboy3D》、《chribocchi》
* 5.重新设计的Game子系统，使用canvas呈现游戏图像
* 6.更新PageManager v1.6
* 7.支持使用SysConfig.h配置软件I2C引脚
* 8.改进theme初始化依赖，watchx_theme与lv_conf.h解耦
* 9.添加EEPROM_File库，支持使用文件模拟EEPROM
* 10.AppWimdow使用lv_obj而不使用lv_cont
* 11.添加MCU和Sim共用的显示初始化函数Display_CommonInit()
* 12.使用DisplayPrivate.h导入字体
* 13.将port层统一添加至lv_port文件组
* 14.关闭背光调整页面的slider动画
