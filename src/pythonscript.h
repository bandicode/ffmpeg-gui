// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_PYTHONSCRIPT_H
#define FFMPEG_GUI_PYTHONSCRIPT_H

#include <fstream>
#include <string>
#include <vector>

class Job;

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
  void osRemove(const std::string& expr);
  void If(const std::string& expr);
  void EndIf();
  void While(const std::string& expr);
  void EndWhile();
  void Call(const std::string& func);
  void Call(const std::string& func, const std::vector<std::string>& args);
  void callIfMain(const std::string& func);

  template<typename...T>
  void Call(const std::string& func, const std::string& arg1, const T&... rest)
  {
    Call(func, std::vector<std::string>{ arg1, rest... });
  }

  void CallFFmpeg(std::vector<std::string> args);

  static std::string str(const std::string& content, char quote = '"');
  static void quoteAll(std::vector<std::string>& list, char quote = '"');
  static std::string Array(const std::vector<std::string>& args);
  static std::string fun(const Job& job);

  static std::string Not(const std::string& expr);
  static std::string osPathExists(const std::string& expr);
  
private:
  std::ofstream m_fstream;
  std::vector<Scope> m_stack;
};

#endif // FFMPEG_GUI_PYTHONSCRIPT_H
