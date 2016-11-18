#encoding=utf8
'''
利用字典暴力破解zip文件的密码
'''
import zipfile
import optparse

def extract_file(file, password):
    try:
        file.extractall('./tmp/', pwd=password)
        return password
    except:
        return

def main():
    opt_parser = optparse.OptionParser('Usage:\nunzip_with_password.py -f <zipfile> -d <dictionary>')
    opt_parser.add_option('-f', dest='zname', type='string', help='specify zip file')
    opt_parser.add_option('-d', dest='dname', type='string', help='specifiy dictionary file')
    (options, args) = opt_parser.parse_args()
    if (options.zname == None) | (options.dname == None):
        print opt_parser.usage
        exit(0)
    else:
        zname = options.zname
        dname = options.dname

    file = zipfile.ZipFile(zname)
    pass_file = open(dname)
    for line in pass_file.readlines():
        password = line.strip('\n')
        guess = extract_file(file, password)
        if guess:
            print '[+] Password = ' + password + '\n'
            exit(0)

if __name__ == '__main__':
    main()
