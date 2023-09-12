#!/usr/bin/env -S python3 -B -u

# == Prepare environemnt ==
#
# ```
# python3 -m pip install faker
# ```
#

from random import randint
from faker import Faker

fake = Faker()
for _ in range(2000):
    #
    key = randint(1, 1000)
    print(key)
    #
    value = fake.first_name()
    print(value)
