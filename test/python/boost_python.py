#!/usr/bin/env python

import sys
sys.path.append('.libs')
import libpyboost

myWorld = libpyboost.World()
myWorld.add('Bonjour', 5)
myWorld.add('Hello', 7)
myWorld.add('Gruss Gott', 5)

#MsgList=myWorld.getMsgList()
#IDList=myWorld.getIDList()

#print ', '.join(['%s: %s' % (msg, id) for (msg, id) in zip(MsgList, IDList)])

a = myWorld.toSimpleString().split(',')
print '^'.join(a[::-1])
