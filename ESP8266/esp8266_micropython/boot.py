# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
import gc
import webrepl
import network
import esp
esp.osdebug(None)
webrepl.start()
gc.collect()

st_if = network.WLAN(network.STA_IF)
ap_if = network.WLAN(network.AP_IF)

ap_if.ifconfig(('6.6.6.1', '255.255.255.128', '6.6.6.1', '8.8.8.8'))
ap_if.config(essid='esp_mpc')
ap_if.config(password='security123')

