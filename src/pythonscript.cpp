// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "pythonscript.h"

#include "job.h"

#include <array>
#include <cassert>

PythonScript::PythonScript(const std::string& file_path)
  : m_fstream(file_path)
{

}

PythonScript::PythonScript(const Job& job, const std::string& output_folder)
  : m_fstream(output_folder + "/job_" + std::to_string(job.num()) + ".py")
{

}

PythonScript::~PythonScript()
{

}

const std::string& PythonScript::indent(size_t size)
{
  static const std::array<std::string, 10> indent_table = {
    "",
    "  ",
    "    ",
    "      ",
    "        ",
    "          ",
    "            ",
    "              ",
    "                ",
    "                  ",
  };

  return indent_table[size];
}

const std::string& PythonScript::indent() const
{
  return indent(m_stack.size());
}

void PythonScript::newline()
{
  m_fstream << "\n";
}

void PythonScript::importSubprocess()
{
  m_fstream << indent() << "import subprocess" << "\n";
}

void PythonScript::importOs()
{
  m_fstream << indent() << "import os" << "\n";
}

void PythonScript::importOsPath()
{
  m_fstream << indent() << "import os.path" << "\n";
}

void PythonScript::def(const std::string& func)
{
  m_fstream << indent() << "def " << func << "():" << "\n";
  m_stack.push_back(FUNCTION);
}

void PythonScript::enddef()
{
  assert(m_stack.back() == FUNCTION);
  m_stack.pop_back();
}

void PythonScript::osRemove(const std::string& expr)
{
  m_fstream << indent() << "os.remove(" + expr + ")" << "\n";
}

void PythonScript::If(const std::string& expr)
{
  m_fstream << indent() << "if " << expr << ":" << "\n";
  m_stack.push_back(IF);
}

void PythonScript::EndIf()
{
  assert(m_stack.back() == IF);
  m_stack.pop_back();
}

void PythonScript::While(const std::string& expr)
{
  m_fstream << indent() << "while " << expr << ":" << "\n";
  m_stack.push_back(WHILE);
}

void PythonScript::EndWhile()
{
  assert(m_stack.back() == WHILE);
  m_stack.pop_back();
}

void PythonScript::callIfMain(const std::string& func)
{
  If("__name__ == \"__main__\"");
  Call(func);
  EndIf();
}

void PythonScript::Call(const std::string& func)
{
  m_fstream << indent() << func << "()" << "\n";
}

void PythonScript::Call(const std::string& func, const std::vector<std::string>& args)
{
  if (args.empty())
  {
    Call(func);
    return;
  }

  m_fstream << indent() << func << "(";

  for (size_t i(0); i < args.size() - 1; ++i)
  {
    m_fstream << args.at(i) << ", ";
  }

  m_fstream << args.back() << ")" << "\n";
}

void PythonScript::CallFFmpeg(std::vector<std::string> args)
{
  quoteAll(args);
  args.insert(args.begin(), "\"ffmpeg\"");
  Call("subprocess.call", Array(args));
}

std::string PythonScript::str(const std::string& content, char quote)
{
  return quote + content + quote;
}

void PythonScript::quoteAll(std::vector<std::string>& list, char quote)
{
  for (std::string& s : list)
  {
    s.insert(s.begin(), quote);
    s.push_back(quote);
  }
}

std::string PythonScript::Array(const std::vector<std::string>& args)
{
  if (args.empty())
  {
    return "[]";
  }

  std::string ret = "[";

  for (size_t i(0); i < args.size() - 1; ++i)
  {
    ret += args.at(i) + ", ";
  }

  ret += args.back() + "]";

  return ret;
}

std::string PythonScript::fun(const Job& job)
{
  return "run_job_" + std::to_string(job.num());
}

std::string PythonScript::Not(const std::string& expr)
{
  return "not " + expr;
}

std::string PythonScript::osPathExists(const std::string& expr)
{
  return "os.path.exists(" + expr + ")";
}
