#!/usr/bin/python

#
# Copyright (C) 2012 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

"""
A parser for metadata_definitions.xml can also render the resulting model
over a Mako template.

Usage:
  metadata_parser_xml.py <filename.xml> <template.mako> [<output_file>]
  - outputs the resulting template to output_file (stdout if none specified)

Module:
  The parser is also available as a module import (MetadataParserXml) to use
  in other modules.

Dependencies:
  BeautifulSoup - an HTML/XML parser available to download from
          http://www.crummy.com/software/BeautifulSoup/
  Mako - a template engine for Python, available to download from
     http://www.makotemplates.org/
"""

import sys
import os
import StringIO

from bs4 import BeautifulSoup
from bs4 import NavigableString

from mako.template import Template
from mako.lookup import TemplateLookup
from mako.runtime import Context

from metadata_model import *
import metadata_model
from metadata_validate import *
import metadata_helpers

class MetadataParserXml:
  """
  A class to parse any XML block that passes validation with metadata-validate.
  It builds a metadata_model.Metadata graph and then renders it over a
  Mako template.

  Attributes (Read-Only):
    soup: an instance of BeautifulSoup corresponding to the XML contents
    metadata: a constructed instance of metadata_model.Metadata
  """
  def __init__(self, xml, file_name):
    """
    Construct a new MetadataParserXml, immediately try to parse it into a
    metadata model.

    Args:
      xml: The XML block to use for the metadata
      file_name: Source of the XML block, only for debugging/errors

    Raises:
      ValueError: if the XML block failed to pass metadata_validate.py
    """
    self._soup = validate_xml(xml)

    if self._soup is None:
      raise ValueError("%s has an invalid XML file" % (file_name))

    self._metadata = Metadata()
    self._parse()
    self._metadata.construct_graph()

  @staticmethod
  def create_from_file(file_name):
    """
    Construct a new MetadataParserXml by loading and parsing an XML file.

    Args:
      file_name: Name of the XML file to load and parse.

    Raises:
      ValueError: if the XML file failed to pass metadata_validate.py

    Returns:
      MetadataParserXml instance representing the XML file.
    """
    return MetadataParserXml(file(file_name).read(), file_name)

  @property
  def soup(self):
    return self._soup

  @property
  def metadata(self):
    return self._metadata

  @staticmethod
  def _find_direct_strings(element):
    if element.string is not None:
      return [element.string]

    return [i for i in element.contents if isinstance(i, NavigableString)]

  @staticmethod
  def _strings_no_nl(element):
    return "".join([i.strip() for i in MetadataParserXml._find_direct_strings(element)])

  def _parse(self):

    tags = self.soup.tags
    if tags is not None:
      for tag in tags.find_all('tag'):
        self.metadata.insert_tag(tag['id'], tag.string)

    types = self.soup.types
    if types is not None:
      for tp in types.find_all('typedef'):
        languages = {}
        for lang in tp.find_all('language'):
          languages[lang['name']] = lang.string

        self.metadata.insert_type(tp['name'], 'typedef', languages=languages)

    # add all entries, preserving the ordering of the XML file
    # this is important for future ABI compatibility when generating code
    entry_filter = lambda x: x.name == 'entry' or x.name == 'clone'
    for entry in self.soup.find_all(entry_filter):
      if entry.name == 'entry':
        d = {
              'name': fully_qualified_name(entry),
              'type': entry['type'],
              'kind': find_kind(entry),
              'type_notes': entry.attrs.get('type_notes')
            }

        d2 = self._parse_entry(entry)
        insert = self.metadata.insert_entry
      else:
        d = {
           'name': entry['entry'],
           'kind': find_kind(entry),
           'target_kind': entry['kind'],
          # no type since its the same
          # no type_notes since its the same
        }
        d2 = {}
        if 'hal_version' in entry.attrs:
          d2['hal_version'] = entry['hal_version']

        insert = self.metadata.insert_clone

      d3 = self._parse_entry_optional(entry)

      entry_dict = dict(d.items() + d2.items() + d3.items())
      insert(entry_dict)

    self.metadata.construct_graph()

  def _parse_entry(self, entry):
    d = {}

    #
    # Visibility
    #
    d['visibility'] = entry.get('visibility')

    #
    # Synthetic ?
    #
    d['synthetic'] = entry.get('synthetic') == 'true'

    #
    # Permission needed ?
    #
    d['permission_needed'] = entry.get('permission_needed')

    #
    # Hardware Level (one of limited, legacy, full)
    #
    d['hwlevel'] = entry.get('hwlevel')

    #
    # Deprecated ?
    #
    d['deprecated'] = entry.get('deprecated') == 'true'

    #
    # Optional for non-full hardware level devices
    #
    d['optional'] = entry.get('optional') == 'true'

    #
    # Typedef
    #
    d['type_name'] = entry.get('typedef')

    #
    # Initial HIDL HAL version the entry was added in
    d['hal_version'] = entry.get('hal_version')

    #
    # Enum
    #
    if entry.get('enum', 'false') == 'true':

      enum_values = []
      enum_deprecateds = []
      enum_optionals = []
      enum_hiddens = []
      enum_ndk_hiddens = []
      enum_notes = {}
      enum_sdk_notes = {}
      enum_ndk_notes = {}
      enum_ids = {}
      enum_hal_versions = {}
      for value in entry.enum.find_all('value'):

        value_body = self._strings_no_nl(value)
        enum_values.append(value_body)

        if value.attrs.get('deprecated', 'false') == 'true':
          enum_deprecateds.append(value_body)

        if value.attrs.get('optional', 'false') == 'true':
          enum_optionals.append(value_body)

        if value.attrs.get('hidden', 'false') == 'true':
          enum_hiddens.append(value_body)

        if value.attrs.get('ndk_hidden', 'false') == 'true':
          enum_ndk_hiddens.append(value_body)

        notes = value.find('notes')
        if notes is not None:
          enum_notes[value_body] = notes.string

        sdk_notes = value.find('sdk_notes')
        if sdk_notes is not None:
          enum_sdk_notes[value_body] = sdk_notes.string

        ndk_notes = value.find('ndk_notes')
        if ndk_notes is not None:
          enum_ndk_notes[value_body] = ndk_notes.string

        if value.attrs.get('id') is not None:
          enum_ids[value_body] = value['id']

        if value.attrs.get('hal_version') is not None:
          enum_hal_versions[value_body] = value['hal_version']

      d['enum_values'] = enum_values
      d['enum_deprecateds'] = enum_deprecateds
      d['enum_optionals'] = enum_optionals
      d['enum_hiddens'] = enum_hiddens
      d['enum_ndk_hiddens'] = enum_ndk_hiddens
      d['enum_notes'] = enum_notes
      d['enum_sdk_notes'] = enum_sdk_notes
      d['enum_ndk_notes'] = enum_ndk_notes
      d['enum_ids'] = enum_ids
      d['enum_hal_versions'] = enum_hal_versions
      d['enum'] = True

    #
    # Container (Array/Tuple)
    #
    if entry.attrs.get('container') is not None:
      container_name = entry['container']

      array = entry.find('array')
      if array is not None:
        array_sizes = []
        for size in array.find_all('size'):
          array_sizes.append(size.string)
        d['container_sizes'] = array_sizes

      tupl = entry.find('tuple')
      if tupl is not None:
        tupl_values = []
        for val in tupl.find_all('value'):
          tupl_values.append(val.name)
        d['tuple_values'] = tupl_values
        d['container_sizes'] = len(tupl_values)

      d['container'] = container_name

    return d

  def _parse_entry_optional(self, entry):
    d = {}

    optional_elements = ['description', 'range', 'units', 'details', 'hal_details', 'ndk_details',\
                         'deprecation_description']
    for i in optional_elements:
      prop = find_child_tag(entry, i)

      if prop is not None:
        d[i] = prop.string

    tag_ids = []
    for tag in entry.find_all('tag'):
      tag_ids.append(tag['id'])

    d['tag_ids'] = tag_ids

    return d

  def render(self, template, output_name=None, hal_version="3.2"):
    """
    Render the metadata model using a Mako template as the view.

    The template gets the metadata as an argument, as well as all
    public attributes from the metadata_helpers module.

    The output file is encoded with UTF-8.

    Args:
      template: path to a Mako template file
      output_name: path to the output file, or None to use stdout
      hal_version: target HAL version, used when generating HIDL HAL outputs.
                   Must be a string of form "X.Y" where X and Y are integers.
    """
    buf = StringIO.StringIO()
    metadata_helpers._context_buf = buf
    metadata_helpers._hal_major_version = int(hal_version.partition('.')[0])
    metadata_helpers._hal_minor_version = int(hal_version.partition('.')[2])

    helpers = [(i, getattr(metadata_helpers, i))
                for i in dir(metadata_helpers) if not i.startswith('_')]
    helpers = dict(helpers)

    lookup = TemplateLookup(directories=[os.getcwd()])
    tpl = Template(filename=template, lookup=lookup)

    ctx = Context(buf, metadata=self.metadata, **helpers)
    tpl.render_context(ctx)

    tpl_data = buf.getvalue()
    metadata_helpers._context_buf = None
    buf.close()

    if output_name is None:
      print tpl_data
    else:
      file(output_name, "w").write(tpl_data.encode('utf-8'))

#####################
#####################

if __name__ == "__main__":
  if len(sys.argv) <= 2:
    print >> sys.stderr,                                                       \
           "Usage: %s <filename.xml> <template.mako> [<output_file>] [<hal_version>]"          \
           % (sys.argv[0])
    sys.exit(0)

  file_name = sys.argv[1]
  template_name = sys.argv[2]
  output_name = sys.argv[3] if len(sys.argv) > 3 else None
  hal_version = sys.argv[4] if len(sys.argv) > 4 else "3.2"

  parser = MetadataParserXml.create_from_file(file_name)
  parser.render(template_name, output_name, hal_version)

  sys.exit(0)
