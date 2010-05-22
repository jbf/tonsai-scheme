#!/usr/bin/env ruby

require 'stringio'

f = File.new("test/sanity.test")

failed = false
f.each do | line |
  m = /(.*),([^#]*)/.match(line)
  cmd = "echo \'#{m[1]}\' | ./repl 2>/dev/null"
  want = m[2].strip
  ret = `#{cmd}`
  begin
    l = StringIO.new(ret).readlines.last.chomp
  rescue
  end
  puts "Test failed: #{m[1]} #{cmd} got: #{l} want: #{want}" unless (want == l)
  failed = true unless (want == l)
end

puts "All tests pass" unless failed
