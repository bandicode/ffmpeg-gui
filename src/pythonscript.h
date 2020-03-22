// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_PYTHONSCRIPT_H
#define FFMPEG_GUI_PYTHONSCRIPT_H

#include <fstream>
#include <string>
#include <vector>

class Job;

class PythonExpression
{
public:
  PythonExpression(): m_str("None") { }
  PythonExpression(const PythonExpression&) = default;
  PythonExpression(PythonExpression&&) noexcept = default;
  ~PythonExpression() = default;

  const std::string& str() const;

  static PythonExpression str(std::string str, char quote = '"');
  static PythonExpression fromString(std::string str);

  PythonExpression& operator=(const PythonExpression&) = default;
  PythonExpression& operator=(PythonExpression&&) noexcept = default;

protected:
  PythonExpression(std::string str)
    : m_str(std::move(str))
  {

  }

private:
  std::string m_str;
};

class PythonScript
{
public:
  explicit PythonScript(const std::string& file_path);
  PythonScript(const Job& job, const std::string& output_folder);
  ~PythonScript();

  enum Scope
  {
    FUNCTION,
    IF,
    FOR,
    WHILE,
  };

  static const std::string& indent(size_t size);
  const std::string& indent() const;

  void newline();
  void importSubprocess();
  void importOs();
  void importOsPath();
  void def(const std::string& func);
  void enddef();
  void osRemove(const PythonExpression& expr);
  void If(const PythonExpression& expr);
  void EndIf();
  void While(const PythonExpression& expr);
  void EndWhile();
  void Call(const std::string& func);
  void Call(const std::string& func, const std::vector<PythonExpression>& args);
  void callIfMain(const std::string& func);

  template<typename...T>
  void Call(const std::string& func, const PythonExpression& arg1, const T&... rest)
  {
    Call(func, std::vector<PythonExpression>{ arg1, rest... });
  }

  void CallFFmpeg(std::vector<std::string> args);

  static PythonExpression str(const std::string& content, char quote = '"');
  static PythonExpression expr(const std::string& content);
  static void quoteAll(std::vector<std::string>& list, char quote = '"');
  static PythonExpression Array(const std::vector<PythonExpression>& args);
  static std::string fun(const Job& job);

  static PythonExpression Not(const PythonExpression& expr);
  static PythonExpression osPathExists(const PythonExpression& expr);
  
private:
  std::ofstream m_fstream;
  std::vector<Scope> m_stack;
};

#endif // FFMPEG_GUI_PYTHONSCRIPT_H
