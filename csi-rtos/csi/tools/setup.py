#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
from codecs import open  # To use a consistent encoding

from setuptools import setup, find_packages  # Always prefer setuptools over distutils

APP_NAME = 'chiptools'
VERSION = '1.0.00'

settings = dict()

from distutils.core import Extension
from distutils.command.build_ext import build_ext as _build_ext

class build_ext(_build_ext):
    def run(self):
        pass


settings.update(
    name = APP_NAME,
    version = VERSION,
    description = 'Chip Tools',
    author = 'Zhuzhg',
    author_email = 'zzg@ifnfn.com',
    packages=find_packages(),
    download_url='http://www.china.com',
    # packages = ['csi'],
    install_requires=[
        'pyyaml>=3.0.0', 'threadpool>=0.0.0', 'GitPython>=2.0.0'
    ],

    license = 'BSD',
    classifiers = [
        'Development Status :: 5 - Production/Stable',
        'Intended Audience :: Developers',
        'Natural Language :: English',
        'License :: OSI Approved :: BSD License',
        'Programming Language :: Python',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.4',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
    ],
    data_files=[
    ],
    entry_points = {
        'console_scripts': [
            'chip = csi.cmd:main',
        ],
    },
    python_requires='>=2.7, !=3.0.*, !=3.1.*, !=3.2.*, !=3.3.*, !=3.4.*',
)

setup(**settings)
