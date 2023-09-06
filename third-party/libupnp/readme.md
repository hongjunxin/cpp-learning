## xml 字段解释

### deviceType
deviceType 的格式通常是一个唯一的URN（Uniform Resource Name）。通常包括以下几个方面：
* 设备类别（Device Category）： deviceType 可以表示设备所属的主要类别。例如，urn:schemas-upnp-org:device:MediaServer:1 表示媒体服务器设备类别。
* 协议版本（Protocol Version）： 有时，deviceType 中包含协议版本信息，以便区分不同版本的设备。例如，:1 表示协议的第一个版本。
* 厂商特定信息： 一些 deviceType 中可能包含制造商或厂商的标识信息，以区分不同制造商的设备。

 ## 搜索目标
UpnpSearchAsync() 第三个字段填写的是搜索目标。搜索目标包括：
* upnp:rootdevice：用于 UPnP 设备发现的标准搜索目标，允许控制点查找并识别网络中的根设备。
* ssdp:all：用于搜索局域网内所有可用的 UPnP 设备的搜索目标。它将返回所有设备的响应，包括根设备、服务和子设备。
* 设备类型搜索目标：例如，urn:schemas-upnp-org:device:MediaServer:1 表示搜索媒体服务器设备。这允许控制点仅查找特定类型的设备。
* 服务类型搜索目标：例如，urn:schemas-upnp-org:service:AVTransport:1 表示搜索支持 AV 传输服务的设备。这允许控制点查找特定功能的设备。
* 根设备 UUID：控制点可以使用特定设备的唯一服务名（USN）来搜索该设备。
* 自定义搜索目标：根据特定应用程序或需求，可以定义自定义搜索目标。

### 搜索原理
使用 http 协议，发送 M-SEARCH 请求
```http
M-SEARCH * HTTP/1.1
Host: 239.255.255.250:1900
Man: "ssdp:discover"
MX: 3
ST: upnp:rootdevice
```
响应
```http
HTTP/1.1 200 OK
CACHE-CONTROL: max-age=1800
EXT:
LOCATION: http://192.168.1.100:50001/desc.xml
SERVER: Some UPnP Device/1.0 UPnP/1.0
ST: upnp:rootdevice
USN: uuid:SomeDeviceUDN::upnp:rootdevice
```

估计可能用 udp 来发送 http 协议，但 curl 只支持通过 tcp 来发送。改用 nc 命令。

```sh
echo -e "M-SEARCH * HTTP/1.1\r\nHost: 239.255.255.250:1900\r\nMan: \"ssdp:discover\"\r\nMX: 5\r\nST: upnp:rootdevice\r\n\r\n" | nc -u -w3 239.255.255.250 1900
```

以上命令的执行结果，tcpdump 抓包结果显示已接收到响应包，但是 nc 没有任何的响应输出。
