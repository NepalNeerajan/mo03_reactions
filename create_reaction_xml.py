# Copyright 2018-2020 Clemson University
#
# Author: Bradley S. Meyer
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# To obtain a copy of the GNU General Public License,
# see <https://www.gnu.org/licenses/>.

import argparse
from lxml import etree

def single_rate_entry(file, data):
    data.text = file.readline().rstrip()
    file.readline()

def table_entry(file, data):
    n_entries = int(file.readline())
    for n in range(n_entries):
        line = file.readline()
        words = line.split()
        if len(words) == 2:
            words.append("1")
        point = etree.SubElement(data, "point")
        t9 = etree.SubElement(point, "t9")
        t9.text = words[0]
        rate = etree.SubElement(point, "rate")
        rate.text = words[1]
        sef = etree.SubElement(point, "sef")
        sef.text = words[2]
    file.readline()

def non_smoker_fit(file, data):
    def read_data(file):
        line = file.readline()
        words = line.split()
        result = {}
        result['Zt'] = words[0]
        result['At'] = words[1]
        result['Zf'] = words[2]
        result['Af'] = words[3]
        result['Q'] = words[4]
        result['spint'] = words[5]
        result['spinf'] = words[6]
        result['TlowHf'] = words[7]
        result['Tlowfit'] = words[8]
        result['Thighfit'] = "10"
        result['acc'] = words[9]

        line = file.readline()
        words = line.split()
        result['a1'] = words[0]
        result['a2'] = words[1]
        result['a3'] = words[2]
        result['a4'] = words[3]
       
        line = file.readline()
        words = line.split()
        result['a5'] = words[0]
        result['a6'] = words[1]
        result['a7'] = words[2]
        result['a8'] = words[3]

        file.readline()

        return result

    n_fits = int(file.readline())

    rd = [
          'Zt', 'At', 'Zf', 'Af', 'Q',
          'spint', 'spinf', 'TlowHf', 'Tlowfit', 'Thighfit', 'acc',
          'a1', 'a2', 'a3', 'a4', 'a5', 'a6', 'a7', 'a8']

    if n_fits == 1:
        file.readline()
        result = read_data(file)
        for r in rd:
            d = etree.SubElement(data, r)
            d.text = result[r]
    else:
        for fit in range(n_fits):
            f = etree.SubElement(data, "fit")
            line = file.readline().rstrip()
            if line == "":
                line = file.readline().rstrip()
            f.set('note', line)
            result = read_data(file)
            for r in rd:
                d = etree.SubElement(f, r)
                d.text = result[r]
        
def user_rate(file, data, delimiter):
    props = etree.SubElement(data, "properties")
    n_props = int(file.readline())
    n_tags = int(file.readline())
    for m in range(n_props):
        line = file.readline()
        if not delimiter:
            words = line.split()
        else:
            words = line.split(delimiter)
        prop = etree.SubElement(props, "property")
        prop.set("name", words[0])
        s_prop = ""
        if n_tags == 0 or words[0] == "note":
            for n in range(1, len(words)):
                s_prop += " " + words[n].strip()
        elif n_tags == 1:
            prop.set("tag1", words[1].strip())
            for n in range(2, len(words)):
                s_prop += " " + words[n].strip()
        elif n_tag2 == 2:
            prop.set("tag1", words[1].strip())
            prop.set("tag2", words[2].strip())
            for n in range(3, len(words)):
                s_prop += " " + words[n].strip()
        else:
            print('Invalid number of property tags')
            exit()
        prop.text = s_prop.lstrip(' ')
    file.readline()

def main():
            
    parser = argparse.ArgumentParser(
                description='Convert a reaction text file to XML'
             )

    parser.add_argument('in_text', metavar='in_text', help='input text file')
    parser.add_argument('out_xml', metavar='out_xml', help='output xml file')
    parser.add_argument('--user_delim', metavar='DELIM',
                        type=str, default="",
                        help="delimiter for user rate properties")

    args = parser.parse_args()

    root = etree.Element("reaction_data")

    file = open(args.in_text)

    while True:
        reaction_type = file.readline().rstrip()

        if len(reaction_type) == 0:
            break

        reaction = etree.SubElement(root, "reaction")

        if reaction_type == 'user_supplied_fit':
            key = file.readline().rstrip()

        source = etree.SubElement(reaction, "source")
        source.text = file.readline().rstrip()

        n_reactants = int(file.readline())

        for n in range(n_reactants):
            reactant = etree.SubElement(reaction, "reactant")
            reactant.text = file.readline().rstrip()

        n_products = int(file.readline())

        for n in range(n_products):
            product = etree.SubElement(reaction, "product")
            product.text = file.readline().rstrip()

        if reaction_type == 'single_rate':
            data = etree.SubElement(reaction, "single_rate")
            single_rate_entry(file, data)
        elif reaction_type == 'rate_table':
            data = etree.SubElement(reaction, "rate_table")
            table_entry(file, data)
        elif reaction_type == 'non_smoker_fit':
            data = etree.SubElement(reaction, "non_smoker_fit")
            non_smoker_fit(file, data)
        elif reaction_type == 'user_supplied_fit':
            data = etree.SubElement(reaction, "user_rate")
            data.set('key', key)
            user_rate(file, data, args.user_delim)

    et = etree.ElementTree(root)
    with open(args.out_xml, 'wb') as out_file:
        et.write(out_file, pretty_print=True)

if __name__ == "__main__":
    main()
