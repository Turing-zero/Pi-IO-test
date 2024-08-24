import sys
sys.path.append("../build/")

import debugger

test = debugger.Debugger()
test.open_device(debugger.Device.BUTTON)