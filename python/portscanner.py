#coding: utf-8

import nmap
import optparse

def nmap_scan(target_host, target_port):
    scanner = nmap.PortScanner()
    scanner.scan(target_host, target_port)
    state = scanner[target_host]['tcp'][int(target_port)]['state']
    print ' [*] ' + target_host + ' tcp/' + target_port + ' ' + state

def main():
    opt_parser = optparse.OptionParser('Usage:\n')
    opt_parser.add_option('-H', dest='target_host', type='string', help='specify target host')
    opt_parser.add_option('-p', dest='target_ports', type='string', help='specify target port(s) separated by comma')
    (options, args) = opt_parser.parse_args()
    target_host = options.target_host
    target_ports = str(options.target_ports).split(', ')
    if (target_host == None) | (target_ports[0] == None):
        print opt_parser.usage()
        exit(0)
    for port in target_ports:
        nmap_scan(target_host, port)

if __name__ == '__main__':
    main()