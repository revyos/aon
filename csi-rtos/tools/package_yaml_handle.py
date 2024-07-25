import os
import sys

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

def search_board_info_yaml(self, subpath=[]):
    paths = []
    if subpath:
        for sub in subpath:
            p = os.path.join(self.csi_rtos_board_path, sub)
            if os.path.exists(p):
                paths.append(p)
    else:
        paths.append(self.csi_rtos_board_path)

    package_list = []

    while paths:
        path = paths[0]
        filename = os.path.join(path, 'info.yaml')
        if os.path.isfile(filename):
            package_list.append(filename)
        else:
            files = os.listdir(path)
            for file in files:
                p = os.path.join(path, file)
                if os.path.isdir(p):
                    paths.append(p)
        del paths[0]
    return package_list

def get_lint_conetxt(file_path, line_number):
    return linecache.getline(file_path, line_number)

old_str = ';ljkl;sajdfkljalsfj'
new_str = ';ljkl;sajdfkljalsfj'

files = search_board_info_yaml(sys.argv[1], ['.'])
for f in files:
    with open(f, 'r') as r:
        lines = r.readlines()
    with open(f, 'w') as w:
        for i in lines:
            if old_str in i:
                i = new_str
            w.write(i)
    r.close()
    w.close()

