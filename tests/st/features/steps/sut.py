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

import os

@given('we have yasboot executable')
def step_impl(context):
    context.executable = os.environ.get("SUT")

@when('we execute')
def step_impl(context):
    if context.executable == None:
        raise RuntimeError()
    context.sut = pexpect.spawn(context.executable, timeout=1)

@then('stdout contains')
def step_impl(context):
    context.sut.expect(context.text)
