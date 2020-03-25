// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'ffmpeg-gui' project
// For conditions of distribution and use, see copyright notice in LICENSE


#include "parser.h"

#include "media.h"
#include "media/chapter.h"
#include "media/audio.h"
#include "media/subtitle.h"
#include "media/video.h"

#include <QDebug>

#include <regex>

static bool starts_with(const std::string& str, const std::string& prefix)
{
  return prefix.length() <= str.length() && std::memcmp(str.data(), prefix.data(), prefix.size()) == 0;
}

static bool ends_with(const std::string& str, const std::string& suffix)
{
  return suffix.length() <= str.length() 
    && std::memcmp(str.data() + str.size() - suffix.size(), suffix.data(), suffix.size()) == 0;
}

static void trim(std::string& str)
{
  auto it = std::find_if(str.begin(), str.end(), [](char c) {
    return c != ' ';
    });

  str.erase(str.begin(), it);

  while (!str.empty() && str.back() == ' ')
    str.pop_back();
}

static void normalize(std::string& str)
{
  size_t r = 0;
  size_t w = 0;

  while (r < str.size())
  {
    if (str[r] == '\r')
    {
      ++r;
    }
    else
    {
      str[w++] = str[r++];
    }
  }

  str.resize(w);
}

static std::vector<std::string> split(const std::string& str, char sep)
{
  std::vector<std::string> ret;

  size_t off = 0;
  size_t n = str.find(sep, off);

  while (n != std::string::npos)
  {
    ret.push_back(std::string(str.begin() + off, str.begin() + n));
    off = n+1;
    n = str.find(sep, off);
  }

  ret.push_back(std::string(str.begin() + off, str.end()));

  return ret;
}

inline static int conv_digit(char c)
{
  return c - '0';
}

static double parse_duration(const char* begin, const char* end)
{
  // format: "00:03:10.60"
  int hours = conv_digit(begin[0]) * 10 + conv_digit(begin[1]);
  int mins = conv_digit(begin[3]) * 10 + conv_digit(begin[4]);
  int secs = conv_digit(begin[6]) * 10 + conv_digit(begin[7]);
  int msecs = conv_digit(begin[9]) * 100 + conv_digit(begin[10]) * 10;

  return hours * 3600 + mins * 60 + secs + (msecs / 1000.);
}

std::shared_ptr<Media> Parser::parseMediaInfo(std::string ffmpeg_output)
{
  normalize(ffmpeg_output);
  m_current_indent = 0;
  m_current_line = 0;
  m_lines = split(ffmpeg_output, '\n');

  while (!starts_with(peek_line(), "Input"))
  {
    qDebug() << "Ignoring FFMPEG info: " << read_line().data();
  }

  const std::string& header = read_line();

  std::string name = header.substr(header.find("from '") + 6);
  name.erase(name.end() - 2, name.end());

  m_result = std::make_shared<Media>(name);

  parse_input();

  return m_result;
}

bool Parser::parseInputDuration(const std::string& line_output, double& duration)
{
  static const std::string prefix = "  Duration:";

  if (!starts_with(line_output, prefix))
    return false;

  // format: "  Duration: 00:03:10.60"
  duration = parse_duration(line_output.data() + 12, line_output.data() + 23);
  return true;
}

bool Parser::parseProgressTime(const std::string& line_output, double& time)
{
  static const std::string prefix = "time=";

  size_t pos = line_output.rfind(prefix);
  
  if (pos == std::string::npos || pos + 5 + 11 >= line_output.size())
    return false;

  // format: "time=00:03:10.60"
  time = parse_duration(line_output.data() + pos + 5, line_output.data() + pos + 16);
  return true;
}

int Parser::indent(const std::string& str)
{
  int i = 0;

  while (i < str.length() && str.at(i) == ' ') ++i;

  return i;
}

bool Parser::at_end() const
{
  return m_current_line == m_lines.size();
}

const std::string& Parser::read_line()
{
  return m_lines.at(m_current_line++);
}

const std::string& Parser::peek_line() const
{
  return m_lines.at(m_current_line);
}

std::pair<std::string, std::string> Parser::extract_metadata(const std::string& line)
{
  size_t sep = line.find(':');

  std::string key = std::string(line.begin(), line.begin() + sep);
  trim(key);

  std::string value = std::string(line.begin() + sep + 1, line.end());
  trim(value);

  return { std::move(key), std::move(value) };
}

void Parser::parse_input()
{
  m_current_indent = indent(peek_line());
  const int local_indent = m_current_indent;

  while (m_current_indent == local_indent)
  {
    if (starts_with(peek_line(), "  Metadata:"))
    {
      parse_input_metadata();
    }
    else if (starts_with(peek_line(), "  Duration:"))
    {
      parse_input_duration();
    }

    m_current_indent = indent(peek_line());
  }

  while (m_current_indent == 4)
  {
    if (starts_with(peek_line(), "    Chapter"))
    {
      parse_chapter();
    }
    else if (starts_with(peek_line(), "    Stream"))
    {
      parse_stream();
    }

    m_current_indent = indent(peek_line());
  }
}
void Parser::parse_input_metadata()
{
  read_line();

  int ind = indent(peek_line());

  while (ind > m_current_indent)
  {
    const std::string& line = read_line();

    auto md = extract_metadata(line);

    if (md.first == "title")
    {
      m_result->title() = md.second;
    }

    ind = indent(peek_line());
  }
}

void Parser::parse_input_duration()
{
  const std::string& line = read_line();
  double duration;

  if (parseInputDuration(line, duration))
  {
    m_result->duration() = duration;
  }
}

void Parser::parse_chapter()
{
  const std::string& chap = read_line();

  std::regex regexp{ "#0:(\\d+): start (\\d+\\.\\d+), end (\\d+\\.\\d+)" };

  auto match_begin =
    std::sregex_iterator(chap.begin(), chap.end(), regexp);
  auto match_end = std::sregex_iterator();

  if (std::distance(match_begin, match_end) != 1)
    throw std::runtime_error{ "Could not parse chapter" };

  std::smatch match = *match_begin;
  m_result->chapters().emplace_back(std::stoi(match[1]), std::stod(match[2]), std::stod(match[3]));

  if (starts_with(peek_line(), "    Metadata:"))
  {
    parse_chapter_metadata(m_result->chapters().back());
  }
}

void Parser::parse_chapter_metadata(Chapter& chap)
{
  read_line();

  while (indent(peek_line()) == 6)
  {
    const std::string& line = read_line();

    auto md = extract_metadata(line);

    if (md.first == "title")
    {
      chap.title() = md.second;
    }
  }
}

void Parser::parse_stream()
{
  const std::string& stream = peek_line();
  bool has_language = true;

  std::regex regexp{ "Stream #0:(\\d+)\\(([^\\s]+)\\): ([^\\s]+):" };

  auto match_begin =
    std::sregex_iterator(stream.begin(), stream.end(), regexp);

  if (std::distance(match_begin, std::sregex_iterator()) != 1)
  {
    has_language = false;

    regexp = std::regex{ "Stream #0:(\\d+): ([^\\s]+):" };

    match_begin =
      std::sregex_iterator(stream.begin(), stream.end(), regexp);

    if (std::distance(match_begin, std::sregex_iterator()) != 1)
      throw std::runtime_error{ "Could not parse stream" };
  }

  std::smatch match = *match_begin;

  int stream_num = std::stoi(match[1]);
  const std::string kind = match[has_language ? 3 : 2];
  std::string lang = has_language ? match[2] : std::string();
  
  if (kind == "Video")
  {
    parse_video_stream(stream_num, std::move(lang));
  }
  else if (kind == "Audio")
  {
    parse_audio_stream(stream_num, std::move(lang));
  }
  else if (kind == "Subtitle")
  {
    parse_subtitle_stream(stream_num, std::move(lang));
  }
  else
  {
    skip_unknown_stream();
  }

  if (starts_with(peek_line(), "    Metadata:"))
  {
    parse_stream_metadata();
  }
}

void Parser::parse_video_stream(int num, std::string lang)
{
  const std::string& line = read_line();

  size_t off = line.find("Video:") + 6;
  std::string data{ line.begin() + off, line.end() };
  trim(data);

  std::regex regexp{ "(\\d+)x(\\d+) \\[SAR (\\d+):(\\d+) DAR (\\d+):(\\d+)\\]" };

  auto match_begin =
    std::sregex_iterator(data.begin(), data.end(), regexp);

  if (std::distance(match_begin, std::sregex_iterator()) != 1)
    throw std::runtime_error{ "Could not parse video stream size" };

  std::smatch match = *match_begin;

  std::pair<int, int> size{ std::stoi(match[1]), std::stoi(match[2]) };
  std::pair<int, int> sar{ std::stoi(match[3]), std::stoi(match[4]) };
  std::pair<int, int> dar{ std::stoi(match[5]), std::stoi(match[6]) };

  double fps = -1.;

  std::vector<std::string> fields = split(data, ',');

  for (std::string& field : fields)
  {
    if (ends_with(field, "fps"))
    {
      field.erase(field.end() - 3, field.end());
      trim(field);
      fps = std::stod(field);
    }
  }

  auto stream = std::make_shared<Video>(num, size, sar, dar, fps);
  stream->language() = std::move(lang);
  m_result->streams().push_back(stream);
}

void Parser::parse_audio_stream(int num, std::string lang)
{
  const std::string& line = read_line();

  size_t off = line.find("Audio:") + 6;
  std::string data{ line.begin() + off, line.end() };
  trim(data);

  int samplerate = -1;

  std::vector<std::string> fields = split(data, ',');

  for (std::string& field : fields)
  {
    if (ends_with(field, "Hz"))
    {
      field.erase(field.end() - 2, field.end());
      trim(field);
      samplerate = std::stoi(field);
    }
  }

  auto stream = std::make_shared<Audio>(num, samplerate);
  stream->language() = std::move(lang);
  m_result->streams().push_back(stream);
}

void Parser::parse_subtitle_stream(int num, std::string lang)
{
  const std::string& line = read_line();
  auto stream = std::make_shared<Subtitle>(num);
  stream->language() = std::move(lang);
  m_result->streams().push_back(stream);
}

void Parser::skip_unknown_stream()
{
  throw std::runtime_error{ "Could not parse stream" };
}

void Parser::parse_stream_metadata()
{
  std::shared_ptr<Stream> stream = m_result->streams().back();

  read_line();

  while (indent(peek_line()) == 6)
  {
    const std::string& line = read_line();

    auto md = extract_metadata(line);

    stream->metadata().push_back(std::move(md));
  }
}
