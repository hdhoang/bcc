#include "BPF.h"
#include "catch.hpp"
#include "common.h"

TEST_CASE("test tracepoint parser", "[TracepointParser]") {
  std::string format =
    "name: sys_enter_read\n"
    "ID: 650\n"
    "format:\n"
    "   field:unsigned short common_type;       offset:0;       size:2; signed:0;\n"
    "   field:unsigned char common_flags;       offset:2;       size:1; signed:0;\n"
    "   field:unsigned char common_preempt_count;       offset:3;       size:1; signed:0;\n"
    "   field:int common_pid;   offset:4;       size:4; signed:1;\n"
    "\n"
    "   field:int __syscall_nr; offset:8;       size:4; signed:1;\n"
    "   field:unsigned int fd;  offset:16;      size:8; signed:0;\n"
    "   field:char * buf;       offset:24;      size:8; signed:0;\n"
    "   field:size_t count;     offset:32;      size:8; signed:0;\n"
    "\n"
    "print fmt: \"fd: 0x%08lx, buf: 0x%08lx, count: 0x%08lx\", ((unsigned long)(REC->fd)), ((unsigned long)(REC->buf)), ((unsigned long)(REC->count))\n";

  std::string expected =
    "struct tracepoint__syscalls__sys_enter_read {\n"
    "\tu64 __do_not_use__;\n"
    "\tint __syscall_nr;\n"
    "\tu64 fd;\n"
    "\tchar * buf;\n"
    "\tsize_t count;\n"
    "};\n";

  {
    std::istringstream input(format);
    std::string result = ebpf::parse_tracepoint(input, "syscalls", "sys_enter_read");
    REQUIRE(expected == result);
  }

  format =
    "name: signal_deliver\n"
    "ID: 114\n"
    "format:\n"
    "  field:unsigned short common_type;       offset:0;       size:2; signed:0;\n"
    "  field:unsigned char common_flags;       offset:2;       size:1; signed:0;\n"
    "  field:unsigned char common_preempt_count;       offset:3;       size:1; signed:0;\n"
    "  field:int common_pid;   offset:4;       size:4; signed:1;\n"
    "\n"
    "  field:int sig;  offset:8;       size:4; signed:1;\n"
    "  field:int errno;        offset:12;      size:4; signed:1;\n"
    "  field:int code; offset:16;      size:4; signed:1;\n"
    "  field:unsigned long sa_handler; offset:24;      size:8; signed:0;\n"
    "  field:unsigned long sa_flags;   offset:32;      size:8; signed:0;\n"
    "\n"
    "print fmt: \"sig=%d errno=%d code=%d sa_handler=%lx sa_flags=%lx\", REC->sig, REC->errno, REC->code, REC->sa_handler, REC->sa_flags\n";

  expected =
    "struct tracepoint__signal__signal_deliver {\n"
    "\tu64 __do_not_use__;\n"
    "\tint sig;\n"
    "\tint errno;\n"
    "\tint code;\n"
    "\tunsigned long sa_handler;\n"
    "\tunsigned long sa_flags;\n"
    "};\n";

  {
    std::istringstream input(format);
    std::string result = ebpf::parse_tracepoint(input, "signal", "signal_deliver");
    REQUIRE(expected == result);
  }
}
