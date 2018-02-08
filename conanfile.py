from conans import ConanFile, tools
import os

class NamedTypeConan(ConanFile):
    name = "NamedType"
    version = "1.0.0"
    description = "Strong typing for C++"
    license = "MIT"
    url = "https://github.com/joboccara/NamedType"
    repo_url = "https://github.com/joboccara/NamedType.git"
    author = "Jonathan Boccara (jonathan@fluentcpp.com)"
    exports_sources = "*.hpp", "LICENSE", "README.md"

#    def source(self):
#        tools.download("https://github.com/joboccara/NamedType/archive/v%s.zip" % self.version, "NamedType.zip")
#        tools.unzip("NamedType.zip")

    def package(self):
        self.copy("*.hpp", dst="include")
