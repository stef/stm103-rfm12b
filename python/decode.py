#!/usr/bin/env python

import sys, math
from collections import defaultdict
from bitarray import bitarray

bitrate=213.0

def split_by_n( seq, n ):
    """A generator to divide a sequence into chunks of n units.
       src: http://stackoverflow.com/questions/9475241/split-python-string-every-nth-character"""
    while seq:
        yield seq[:n]
        seq = seq[n:]

def parse(pkt):
    syncbytes=''.join([format(ord(c),"08b") for c in "\xaa\x2d\xe9"])
    start = pkt.to01().find(syncbytes)+len(syncbytes)
    msg=[]
    if start>len(syncbytes):
        for b in split_by_n(pkt.to01()[start:],8):
            msg.append(chr(int(b,2)))
    if len(msg)>0:
        print '\n', len(msg), repr(''.join(msg))

def decode():
    BSIZE = 1 << 16
    mm = sys.stdin.read(BSIZE)
    idx = 0
    bit = ord(mm[0]) ^ 1
    cnt = defaultdict(int)
    total = 0
    block = bitarray()
    while(True):
        # buffered find(bit,idx) from stdin
        pos = mm.find(chr(bit), idx - total if idx>total else 0)
        while pos == -1:
            total += len(mm)
            mm = sys.stdin.read(BSIZE)
            if not mm:
                #print
                #for k, v in sorted(cnt.items()):
                #    print "%.5s %.5s %0.2f" % (k, v, math.fabs((math.modf(k/bitrate))[0] - 0.5) )
                return
            pos = mm.find(chr(bit), (idx - total) if idx>total else 0)
        pos += total

        size = pos - idx
        idx = pos
        bit ^= 1
        if size>10:
            fr = math.fabs((math.modf(size/bitrate))[0] - 0.5)
            bits = int(round(size/bitrate))
            if bits>4000:
                parse(block)
                block=bitarray()
            if bits < 1000 and fr>0.39:
                sys.stdout.write(chr(ord('0') + bit) * bits)
                block.extend(chr(ord('0') + bit) * bits)
            else:
                print "\n\t\t\t", chr(ord('0') + bit), int(round(size/bitrate)), fr
        cnt[size]+=1

if __name__ == '__main__':
    decode()
