#!/usr/bin/env ruby

require 'stringio'

f = File.new("sanity.test")

failed = false
f.each do | line |
  m = /(.*),([^#]*)/.match(line)
  cmd = "echo \'#{m[1]}\' | ../repl 2>/dev/null"
  want = m[2].strip
  ret = `#{cmd}`
  l = StringIO.new(ret).readlines.last.chomp
  puts "Test failed: #{m[1]} #{cmd} #{l} #{want}" unless (want == l)
  failed = true unless (want == l)
end

puts "All tests pass" unless failed
