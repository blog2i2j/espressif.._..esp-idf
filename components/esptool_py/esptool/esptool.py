#
# SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
#
# SPDX-License-Identifier: Apache-2.0
#
import subprocess
import sys

if __name__ == '__main__':
    sys.exit(subprocess.run([sys.executable, '-u', '-m', 'esptool'] + sys.argv[1:]).returncode)
