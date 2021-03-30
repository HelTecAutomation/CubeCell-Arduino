#!/usr/bin/env python

from __future__ import print_function

import os
import shutil
import errno
import os.path
import hashlib
import json
import platform
import sys
import tarfile
import zipfile
import re


if sys.version_info[0] == 3:
    from urllib.request import urlretrieve
    unicode = lambda s: str(s)
else:
    # Not Python 3 - today, it is most likely to be Python 2
    from urllib import urlretrieve
    
if 'Windows' in platform.system():
    import urllib.request
    
current_dir = os.path.dirname(os.path.realpath(unicode(__file__)))
dist_dir = current_dir + '/dist/'

def sha256sum(filename, blocksize=65536):
    hash = hashlib.sha256()
    with open(filename, "rb") as f:
        for block in iter(lambda: f.read(blocksize), b""):
            hash.update(block)
    return hash.hexdigest()

def mkdir_p(path):
    if not os.path.exists(path): 
      os.mkdir(path)
    else:
      for root, dirs, files in os.walk(path, topdown=False):
        for name in files:
            os.remove(os.path.join(root, name))
        for name in dirs:
            os.rmdir(os.path.join(root, name))
      os.removedirs(path)
      os.mkdir(path)
    #try:
    #    os.makedirs(path)
    #except OSError as exc:
     #   if exc.errno != errno.EEXIST or not os.path.isdir(path):
     #       raise

        
def format_size(bytes):
    try:
        bytes = float(bytes)
        kb = bytes / 1024
    except:
        return "Error"
    if kb >= 1024:
        M = kb / 1024
        if M >= 1024:
            G = M / 1024
            return "%.2fG" % (G)
        else:
            return "%.2fM" % (M)
    else:
        return "%.2fK" % (kb)
 

def report_progress(blocknum, blocksize, totalsize):
    percent = int(blocknum*blocksize*100/totalsize)
    percent = min(100, percent)
    totalsize_str = " totalsize: %s" % format_size(totalsize)
    sys.stdout.write("\r%d%%   " % percent+totalsize_str)
    sys.stdout.flush()

def unpack(filename, destination):
    dirname = ''
    print('Extracting {0}'.format(os.path.basename(filename)))
    sys.stdout.flush()
    if filename.endswith('tar.gz'):
        tfile = tarfile.open(filename, 'r:gz')
        tfile.extractall(destination)
        dirname = tfile.getnames()[0]
    elif filename.endswith('zip'):
        zfile = zipfile.ZipFile(filename)
        zfile.extractall(destination)
        dirname = zfile.namelist()[0]
    else:
        raise NotImplementedError('Unsupported archive type')

def get_tool(tool):
    sys_name = platform.system()
    archive_name = tool['archiveFileName']
    local_path = dist_dir + archive_name
    url = tool['url']
    #real_hash = tool['checksum'].split(':')[1]
    if not os.path.isfile(local_path):
        print('Downloading ' + archive_name)
        sys.stdout.flush()
        if 'CYGWIN_NT' in sys_name:
            import ssl
            ctx = ssl.create_default_context()
            ctx.check_hostname = False
            ctx.verify_mode = ssl.CERT_NONE
            urlretrieve(url, local_path, report_progress, context=ctx)
        elif 'Windows' in sys_name:
            urllib.request.urlretrieve(url, local_path, report_progress,)
        else:
            urlretrieve(url, local_path, report_progress)
        sys.stdout.write("\r\nDone\n")
        sys.stdout.flush()
    else:
        print('Tool {0} already downloaded'.format(archive_name))
        sys.stdout.flush()
    #local_hash = sha256sum(local_path)
    #if local_hash != real_hash:
    #    print('Hash mismatch for {0}, delete the file and try again'.format(local_path))
    #    raise RuntimeError()
    unpack(local_path, '.')

def load_tools_list(filename, platform):
    tools_info = json.load(open(filename))['packages'][0]['tools']
    tools_to_download = []
    for t in tools_info:
        tool_platform = [p for p in t['systems'] if p['host'] == platform]
        if len(tool_platform) == 0:
            continue
        tools_to_download.append(tool_platform[0])
    return tools_to_download

def identify_platform():
    arduino_platform_names = {'Darwin'  : {32 : 'i386-apple-darwin',   64 : 'x86_64-apple-darwin'},
                              'Linux'   : {32 : 'i686-pc-linux-gnu',   64 : 'x86_64-pc-linux-gnu'},
                              'LinuxARM': {32 : 'arm-linux-gnueabihf', 64 : 'aarch64-linux-gnu'},
                              'Windows' : {32 : 'i686-mingw32',        64 : 'i686-mingw32'}}
    bits = 32
    if sys.maxsize > 2**32:
        bits = 64
    sys_name = platform.system()
    sys_platform = platform.platform()
    print('System: %s, Info: %s' % (sys_name, sys_platform))
    if 'Linux' in sys_name and sys_platform.find('arm') > 0:
        sys_name = 'LinuxARM'
    if 'CYGWIN_NT' in sys_name:
        sys_name = 'Windows'
    return arduino_platform_names[sys_name][bits]

if __name__ == '__main__':
    identified_platform = identify_platform()
    print('Platform: {0}'.format(identified_platform))
    tools_to_download = load_tools_list(current_dir + '/../package/package_CubeCell_index.template.json', identified_platform)
    mkdir_p(dist_dir)
    for tool in tools_to_download:
        get_tool(tool)
    print('Done')