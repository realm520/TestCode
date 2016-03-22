#!/usr/bin/python

#coding=utf-8

import sys
import urllib
import urllib2
import json

class BaiduWeatherApi(object):
    _api_key = "a7a036cdee1948cb3244a5a5a0ae4dfb"


    def _get_weather(self, url):
        req = urllib2.Request(url)
        req.add_header("apikey", self._api_key)
        resp = urllib2.urlopen(req)
        content = resp.read()
        #if (content):
            #print content
        return content
    

    def city_list(self, city_name):
        url = 'http://apis.baidu.com/apistore/weatherservice/citylist?cityname=' + city_name
        return self._get_weather(url)
    
    def recent_weather(self, city_name, city_id):
        url = 'http://apis.baidu.com/apistore/weatherservice/recentweathers?cityname=' + city_name + '&cityid=' + city_id
        return self._get_weather(url)

    def weather_city_pinyin(self, city_pinyin):
        url = 'http://apis.baidu.com/apistore/weatherservice/weather?citypinyin=' + city_pinyin
        return self._get_weather(url)
    
    def weather_city_name(self, city_name):
        url = 'http://apis.baidu.com/apistore/weatherservice/cityname?cityname=' + city_name
        return self._get_weather(url)

    def weather_city_info(self, city_name):
        url = 'http://apis.baidu.com/apistore/weatherservice/cityinfo?cityname=' + city_name
        return self._get_weather(url)

    def weather_city_id(self, city_id):
        url = 'http://apis.baidu.com/apistore/weatherservice/cityid?cityid=' + city_id
        return self._get_weather(url)

'''
city_list("%E6%9C%9D%E9%98%B3")
recent_weather("%E5%8C%97%E4%BA%AC", "101010100")
weather_pinyin('jiangyin')
weather_city_name('%E5%8C%97%E4%BA%AC')
weather_city_info('%E5%8C%97%E4%BA%AC')
weather_city_id('101010100')
'''

