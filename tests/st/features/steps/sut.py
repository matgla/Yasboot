# -*- coding: utf-8 -*-

#
# sut.py
#
# Copyright (C) 2023 Mateusz Stadnik <matgla@live.com>
#
# This program is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation, either version
# 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General
# Public License along with this program. If not, see
# <https://www.gnu.org/licenses/>.
#

from behave import *
import pexpect
import signal 
import os
import time 

def terminate_sut(context):
    context.sut.kill(signal.SIGINT)
    context.sut.wait()
    context.sut.expect("Yasboot exit")
    assert context.sut.exitstatus == 0

@given('we have yasboot executable')
def step_impl(context):
    context.executable = os.environ.get("SUT")

@when('we execute')
def step_impl(context):
    if context.executable == None:
        raise RuntimeError()
    context.sut = pexpect.spawn(context.executable, timeout=1)
    log_directory = os.environ.get("LOG_DIR")
    if log_directory == None:
        raise RuntimeError("LOG_DIR not provided")
    context.sut.logfile = open(log_directory + "/yasboot.log", "wb") 
    context.add_cleanup(terminate_sut, context)

@then('stdout contains')
def step_impl(context):
    context.sut.expect(context.text)

