## Process this file with automake to produce Makefile.in
# Copyright (C) 2010, 2012 Free Software Foundation, Inc.
#
# This file is part of GnuTLS.
#
# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This file is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this file; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

AM_CFLAGS = $(WARN_CFLAGS) $(WERROR_CFLAGS)
AM_CPPFLAGS = -I$(top_srcdir)/lib/includes		\
	-I$(top_builddir)/lib/includes \
	-I$(top_srcdir)/tests/ \
	-I$(top_srcdir)/lib \
	-I$(top_srcdir)/gl

if ENABLE_MINITASN1
AM_CPPFLAGS += -I$(top_builddir)/lib/minitasn1
endif

AM_LDFLAGS = -no-install
LDADD = ../libutils.la \
	$(top_builddir)/lib/libgnutls.la $(LTLIBGCRYPT) $(LIBSOCKET)

if !ENABLE_SELF_CHECKS
cipher_test_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_builddir)/lib/ -I$(top_builddir)/gl/
else
cipher_test_CPPFLAGS = $(AM_CPPFLAGS)
endif

ctests = gendh keygen cipher-test

check_PROGRAMS = $(ctests)
TESTS = $(ctests)

EXTRA_DIST = README

TESTS_ENVIRONMENT = EXEEXT=$(EXEEXT) \
	LC_ALL="C"						\
	top_builddir="$(top_builddir)"				\
	srcdir="$(srcdir)"
