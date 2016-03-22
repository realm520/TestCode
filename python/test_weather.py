#!/usr/bin/env python
#encoding: utf-8

import unittest
import json
import sys
sys.path.append('/root/code/TestCode/lib/py')
import weather_api

'''
city_list("%E6%9C%9D%E9%98%B3")
recent_weather("%E5%8C%97%E4%BA%AC", "101010100")
weather_pinyin('jiangyin')
weather_city_name('%E5%8C%97%E4%BA%AC')
weather_city_info('%E5%8C%97%E4%BA%AC')
weather_city_id('101010100')
'''

class BaiduWeatherTestCase(unittest.TestCase):
    def test_city_list(self):
        weather = weather_api.BaiduWeatherApi()
        r = weather.city_list('%E6%9C%9D%E9%98%B3')
        rObj = json.loads(r)
        self.assertEqual(rObj['errMsg'], 'success')

    def test_recent_weather(self):
        weather = weather_api.BaiduWeatherApi()
        r = weather.recent_weather('%E5%8C%97%E4%BA%AC', '101010100')
        rObj = json.loads(r)
        self.assertEqual(rObj['errMsg'], 'success')

    def test_weather_city_pinyin(self):
        weather = weather_api.BaiduWeatherApi()
        r = weather.weather_city_pinyin('jiangyin')
        rObj = json.loads(r)
        self.assertEqual(rObj['errMsg'], 'success')

    def test_weather_city_name(self):
        weather = weather_api.BaiduWeatherApi()
        r = weather.weather_city_name('%E5%8C%97%E4%BA%AC')
        rObj = json.loads(r)
        self.assertEqual(rObj['errMsg'], 'success')

    def test_weather_city_info(self):
        weather = weather_api.BaiduWeatherApi()
        r = weather.weather_city_info('%E5%8C%97%E4%BA%AC')
        rObj = json.loads(r)
        self.assertEqual(rObj['retMsg'], 'success')

    def test_weather_city_id(self):
        weather = weather_api.BaiduWeatherApi()
        r = weather.weather_city_id('101010100')
        rObj = json.loads(r)
        self.assertEqual(rObj['errMsg'], 'success')


if __name__ == '__main__':
    unittest.main()
