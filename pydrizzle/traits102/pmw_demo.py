from __future__ import division, print_function # confidence high

import sys
import Pmw
if sys.version_info[0] >= 3:
    import Tkinter as tk
else:
    import tkinter as tk
    
class Demo:
    def __init__(self, parent):
        # Create two buttons to launch the dialog.
        w = tk.Button(parent, text = 'Show application modal dialog',
                command = self.showAppModal)
        w.pack(padx = 8, pady = 8)

        w = tk.Button(parent, text = 'Show global modal dialog',
                command = self.showGlobalModal)
        w.pack(padx = 8, pady = 8)

        w = tk.Button(parent, text = 'Show dialog with "no grab"',
                command = self.showDialogNoGrab)
        w.pack(padx = 8, pady = 8)

        w = tk.Button(parent, text =
                    'Show toplevel window which\n' +
                    'will not get a busy cursor',
                command = self.showExcludedWindow)
        w.pack(padx = 8, pady = 8)

        # Create the dialog.
        self.dialog = Pmw.Dialog(parent,
            buttons = ('OK', 'Apply', 'Cancel', 'Help'),
            defaultbutton = 'OK',
            title = 'My dialog',
            command = self.execute)
        self.dialog.withdraw()

        # Add some contents to the dialog.
        w = tk.Label(self.dialog.interior(),
            text = 'Pmw Dialog\n(put your widgets here)',
            background = 'black',
            foreground = 'white',
            pady = 20)
        w.pack(expand = 1, fill = 'both', padx = 4, pady = 4)

        # Create the window excluded from showbusycursor.
        self.excluded = Pmw.MessageDialog(parent,
            title = 'I still work',
            message_text =
                'This window will not get\n' +
                'a busy cursor when modal dialogs\n' +
                'are activated.  In addition,\n' +
                'you can still interact with\n' +
                'this window when a "no grab"\n' +
                'modal dialog is displayed.')
        self.excluded.withdraw()
        Pmw.setbusycursorattributes(self.excluded.component('hull'),
            exclude = 1)

    def showAppModal(self):
        self.dialog.activate(geometry = 'centerscreenalways')

    def showGlobalModal(self):
        self.dialog.activate(globalMode = 1)

    def showDialogNoGrab(self):
        self.dialog.activate(globalMode = 'nograb')

    def showExcludedWindow(self):
        self.excluded.show()

    def execute(self, result):
        print('You clicked on', result)
        if result not in ('Apply', 'Help'):
            self.dialog.deactivate(result)
