// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FFMPEG_GUI_PARSER_H
#define FFMPEG_GUI_PARSER_H

#include <memory>
#include <string>
#include <vector>

struct Crop;

class Chapter;
class Media;

class Parser
{
public:

  std::shared_ptr<Media> parseMediaInfo(std::string ffmpeg_output);
  static bool parseInputDuration(const std::string& line_output, double& duration);
  static bool parseProgressTime(const std::string& line_output, double& time);
  static Crop parseCrop(const std::string& str);

protected:
  static int indent(const std::string& str);
  bool at_end() const;
  const std::string& read_line();
  const std::string& peek_line() const;
  std::pair<std::string, std::string> extract_metadata(const std::string& line);

  void parse_input();
  void parse_input_metadata();
  void parse_input_duration();
  void parse_chapter();
  void parse_chapter_metadata(Chapter& chap);
  void parse_stream();
  void parse_video_stream(int num, std::string lang);
  void parse_audio_stream(int num, std::string lang);
  void parse_subtitle_stream(int num, std::string lang);
  void skip_unknown_stream();
  void parse_stream_metadata();

private:
  int m_current_indent = 0;
  std::vector<std::string> m_lines;
  size_t m_current_line = 0;
  std::shared_ptr<Media> m_result;
};

#endif // FFMPEG_GUI_PARSER_H
