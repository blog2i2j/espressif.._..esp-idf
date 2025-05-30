# SPDX-FileCopyrightText: 2021-2025 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: CC0-1.0
import pytest
from pytest_embedded import Dut
from pytest_embedded_idf.utils import idf_parametrize


@pytest.mark.generic
@idf_parametrize('target', ['esp32', 'esp32s3', 'esp32c6', 'esp32c5', 'esp32h2', 'esp32p4'], indirect=['target'])
def test_hc_sr04_example(dut: Dut) -> None:
    dut.expect_exact('example: Install capture timer')
    dut.expect_exact('example: Install capture channel')
    dut.expect_exact('example: Register capture callback')
    dut.expect_exact('example: Enable capture channel')
    dut.expect_exact('example: Configure Trig pin')
    dut.expect_exact('example: Enable and start capture timer')
