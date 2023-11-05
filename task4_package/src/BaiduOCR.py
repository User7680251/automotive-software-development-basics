#! /usr/bin/env python3
# -*- coding: UTF-8 -*-
# 来自百度云api官方例程 https://github.com/Baidu-AIP/QuickStart/blob/master/OCR/main.py

import sys
import re
import json
import base64
import cv2
from urllib.request import urlopen
from urllib.request import Request
from urllib.error import URLError
from urllib.parse import urlencode
from urllib.parse import quote_plus


# 防止https证书校验不正确
import ssl
ssl._create_default_https_context = ssl._create_unverified_context

API_KEY = 'iQxqwpkGR8q5kHcuIir9WlRi'

SECRET_KEY = 'v3HD1xvlR59WeWrXpYzxNmwHAQWygB2l'


OCR_URL = "https://aip.baidubce.com/rest/2.0/ocr/v1/general_basic"


"""  TOKEN start """
TOKEN_URL = 'https://aip.baidubce.com/oauth/2.0/token'


"""
    获取token
"""
def fetch_token():
    params = {'grant_type': 'client_credentials',
              'client_id': API_KEY,
              'client_secret': SECRET_KEY}
    post_data = urlencode(params)
    post_data = post_data.encode('utf-8')
    req = Request(TOKEN_URL, post_data)
    try:
        f = urlopen(req, timeout=5)
        result_str = f.read()
    except URLError as err:
        print(err)
    result_str = result_str.decode()


    result = json.loads(result_str)

    if ('access_token' in result.keys() and 'scope' in result.keys()):
        if not 'brain_all_scope' in result['scope'].split(' '):
            print ('please ensure has check the  ability')
            exit()
        return result['access_token']
    else:
        print ('please overwrite the correct API_KEY and SECRET_KEY')
        exit()

"""
    读取文件
"""
def read_file(image_path):
    f = None
    try:
        f = open(image_path, 'rb')
        return f.read()
    except:
        print('read image file fail')
        return None
    finally:
        if f:
            f.close()


"""
    调用远程服务
"""
def request(url, data):
    req = Request(url, data.encode('utf-8'))
    has_error = False
    try:
        f = urlopen(req)
        result_str = f.read()
        result_str = result_str.decode()
        return result_str
    except  URLError as err:
        print(err)

def BaiduOCR(img):
    # 获取access token
    token = fetch_token()

    # 拼接通用文字识别高精度url
    image_url = OCR_URL + "?access_token=" + token

    text = ""

    # 读取图片
    cv_img = img

    # 保存图像到temp.jpg
    save_path = "/home/agilex/catkin_ws/src/automotive-software-development-basics/task4_package/.temp/temp.jpg"
    cv2.imwrite(save_path, cv_img)
    # print("Image saved as temp.jpg")

    # 读取保存的图像文件
    file_content = read_file(save_path)

    # print("type: ", type(file_content))

    # 调用文字识别服务
    result = request(image_url, urlencode({'image': base64.b64encode(file_content)}))

    # 解析返回结果
    try:
        result_json = json.loads(result)
        for words_result in result_json["words_result"]:
            text = text + words_result["words"]
    except Exception as e:
        pass

    # 打印文字
    result = ""
    for char in text:
        if char.isupper() and char in "ABCD":
            result += char

    return result

if __name__ == '__main__':
    BaiduOCR()