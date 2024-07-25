# -*- coding:utf-8 -*-
#
# Copyright (C) 2015-2020 Alibaba Group Holding Limited

from __future__ import print_function
import os
import sys
import time
import hashlib
import zipfile
import codecs
from pathlib import Path
from distutils.dir_util import copy_tree
from operator import itemgetter
import datetime
import glob
import shutil
import xml.etree.ElementTree as ET

try:
    from urlparse import urlparse
    import urllib
    import httplib as http
    urlretrieve = urllib.urlretrieve
except:
    from urllib.parse import urlparse
    import urllib.request
    import http.client as http
    urlretrieve = urllib.request.urlretrieve


try:
    import yaml
except:
    print("\n\nNot found pyyaml, please install: \nsudo pip install pyyaml")
    sys.exit(0)


def yaml_load(filename):
    try:
        with codecs.open(filename) as fh:
            text = fh.read()
            return yaml.safe_load(text)
    except Exception as e:
        print(e)
        exit(-1)


def yaml_dump(dic, filename):
    try:
        file = open(filename,"w")
        yaml.dump(dic, file)
    except Exception as e:
        print("create %s error!" %(filename))
        print(e)
        exit(-1)


def pretty_xml(element, indent, newline, level=0):  # elemnt为传进来的Elment类，参数indent用于缩进，newline用于换行
    if element:  # 判断element是否有子元素
        if (element.text is None) or element.text.isspace():  # 如果element的text没有内容
            element.text = newline + indent * (level + 1)
        else:
            element.text = newline + indent * (level + 1) + element.text.strip() + newline + indent * (level + 1)
            # else:  # 此处两行如果把注释去掉，Element的text也会另起一行
            # element.text = newline + indent * (level + 1) + element.text.strip() + newline + indent * level
    temp = list(element)  # 将element转成list
    for subelement in temp:
        if temp.index(subelement) < (len(temp) - 1):  # 如果不是list的最后一个元素，说明下一个行是同级别元素的起始，缩进应一致
            subelement.tail = newline + indent * (level + 1)
        else:  # 如果是list的最后一个元素， 说明下一行是母元素的结束，缩进应该少一个
            subelement.tail = newline + indent * level
        pretty_xml(subelement, indent, newline, level=level + 1)  # 对子元素进行递归操作

class XmlTree:
    def __init__(self, filename):
        self.tree = self.__xml_load(filename)
        self.root = self.tree.getroot()
    def __xml_load(self, filename):
        try:
            tree = ET.parse(filename)
            return tree
        except Exception as e:
            print('xml load error')
            print(e)
            return None

    def write(self, filename):
        pretty_xml(self.root, '\t', '\n')
        try:
            self.tree.write(filename)
        except Exception as e:
            print('xml write error, file %s' %(filename))
            print(e)
            exit(-1)

    def find(self, tag):
        for v in self.root.iter():
            if v.tag == tag:
                return v
        return None

def current_pwd():
    return os.getenv('PWD')


def home_path(path=''):
    return os.path.join(os.path.expanduser('~'), path)


def MD5(str):
    hl = hashlib.md5()
    hl.update(str.encode(encoding='utf-8'))
    return hl.hexdigest()


def http_get(url, path):
    conn = urlparse(url)

    if conn.scheme == "https":
        connection = http.HTTPSConnection(conn.netloc)
    else:
        connection = http.HTTPConnection(conn.netloc)

    connection.request('GET', conn.path)
    response = connection.getresponse()

    filename = os.path.join(path, os.path.basename(conn.path))

    try:
        with open(filename, 'wb') as f:
            f.write(response.read())
    except:
        pass

    return filename


def wget(url, out_file):
    start_time = time.time()
    def barProcess(blocknum, blocksize, totalsize):
        speed = (blocknum * blocksize) / (time.time() - start_time)
        # speed_str = " Speed: %.2f" % speed
        speed_str = " Speed: %sB/S         " % format_size(speed)
        recv_size = blocknum * blocksize

        # 设置下载进度条
        f = sys.stdout
        pervent = recv_size / totalsize
        percent_str = "%.2f%%" % (pervent * 100)
        n = round(pervent * 50)
        s = ('#' * n).ljust(50, '-')
        f.write(percent_str.ljust(8, ' ') + '[' + s + ']' + speed_str)
        f.flush()
        f.write('\r')

    def format_size(bytes):
        bytes = float(bytes)
        kb = bytes / 1024
        if kb >= 1024:
            M = kb / 1024
            if M >= 1024:
                G = M / 1024
                return "%.3fG" % (G)
            else:
                return "%.3fM" % (M)
        else:
            return "%.3fK" % (kb)

    return urlretrieve(url, out_file, barProcess)


def dfs_get_zip_file(input_path, result):
    files = os.listdir(input_path)
    for file in files:
        if os.path.isdir(input_path + '/' + file):
            dfs_get_zip_file(input_path + '/' + file, result)
        else:
            result.append(input_path + '/' + file)


def zip_path(input_path, zipName):
    if os.path.isdir(input_path):
        base = os.path.dirname(zipName)
        try:
            os.makedirs(base)
        except:
            pass
        predir = input_path.rsplit('/', 1)[0]
        f = zipfile.ZipFile(zipName, 'w', zipfile.ZIP_DEFLATED)
        filelists = []
        dfs_get_zip_file(input_path, filelists)
        for file in filelists:
            suffix = os.path.splitext(file)[-1]
            if suffix != '.d' and suffix != '.o':
                f.write(file, file.split(predir)[1])
        f.close()

def read_file(filename):
    try:
        with open(filename, 'r') as f:
            return f.read()
    except:
        print("open file failed.", filename)
        return ''

def write_file(text, filename):
    contents = None

    try:
        with open(filename, 'r') as f:
            contents = f.read()
    except:
        pass

    if text == contents:
        return
    try:
        p = os.path.dirname(filename)
        try:
            os.makedirs(p)
        except:
            pass
        with open(filename, 'w') as f:
            f.write(text)
    except:
        print("Generate %s file failed." % filename)

def dcit_sorted(dcit, key) :
    return sorted(dcit, key=itemgetter(key))

def mkdir(path):
    folder = os.path.exists(path)
    if folder != True:
        os.makedirs(path)
    else:
        print(path + " is already exist.")

def dir_is_exit(path) :
    if not path:
        return False
    my_dir = Path(path)
    return my_dir.is_dir()

def file_is_exit(file_path):
    if not file_path:
        return False
    my_file = Path(file_path)
    return my_file.is_file()

def copy(src, dst):
    try:
        os.makedirs(dst)
    except:
        pass

    if dir_is_exit(src):
        try:
           copy_tree(src, dst)
        except IOError as e:
           print("Unable to copy file. %s" % e)
        except:
           print("Unexpected error:", sys.exc_info())
           exit(-1)
    elif file_is_exit(src):
        try:
           shutil.copy(src, dst)
        except IOError as e:
           print("Unable to copy file. %s" % e)
        except:
           print("Unexpected error:", sys.exc_info())
           exit(-1)
    else:
        #print('%s not exit' %src)
        pass

def copy_type(src, dst, type):
    f = glob.glob(src + '/*' + type)
    for v in f:
        copy(v, dst)

def remove(path):
    if dir_is_exit(path):
        shutil.rmtree(path)
    elif file_is_exit(path):
        os.remove(path)

def movefile(srcfile, dstfile):
    if not os.path.isfile(srcfile):
        print("move file error, %s not exist!" %(srcfile))
    else:
        fpath, fname = os.path.split(dstfile)
        if not os.path.exists(fpath):
            os.makedirs(fpath)
        shutil.move(srcfile,dstfile)

class CreateCFile:
    def __init__(self):
         self.ali_license = 'Copyright (C) 2017-2019 Alibaba Group Holding Limited'

    def generate_file_description(self, name, brief, version):
        text = '/'+ '*' * 78 + '\n'
        txt_name = ' * @file' + ' ' * 5 + name + '\n'
        txt_version = ' * @version' + ' ' * 2 + version + '\n'

        curr_time = str(datetime.datetime.now())[0:10]
        txt_data = ' * @date' + ' ' * 5 +  curr_time + '\n'

        text += txt_name + brief + txt_version + txt_data
        text += '*' * 78 + '/\n'
        return text

    def generate_license(self, lecense = None):
        if lecense == None:
            lecense = self.ali_license
        text = '/*\n' + ' *' + lecense + '\n' + ' */\n'
        return text

def string_len(text):
    L = 0
    R = ''
    for c in text:
        if c >= '\u4E00' and c <= '\u9FA5':
            L += 2
        else:
            L += 1
    return L


def test():
    c = CreateCFile()
    brief = ''' * @brief    CSI Core Peripheral Access Layer Header File for
 *           CSKYSOC Device Series\n'''

    txt = c.generate_file_description('bbb', brief, 'v1.0')
    print(txt)

if __name__ == "__main__":
    test()

