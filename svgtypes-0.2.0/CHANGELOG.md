# Change Log
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [Unreleased]

## [0.2.0] - 2018-09-12
### Added
- `black`, `white`, `gray`, `red`, `green` and `blue` constructors to the `Color` struct.

### Changed
- `StyleParser` will return `(StrSpan, StrSpan)` and not `StyleToken` from now.
- `StyleParser` requires entity references to be resolved before parsing from now.

### Removed
- `failure` dependency.
- `StyleToken`.
- `Error::InvalidEntityRef`.

## [0.1.1] - 2018-05-23
### Added
- `encoding` and `standalone` to AttributeId.
- `new_translate`, `new_scale`, `new_rotate`, `new_rotate_at`, `new_skew_x`, `new_skew_y`
  and `rotate_at` methods to the `Transform`.

### Changed
- `StreamExt::parse_iri` and `StreamExt::parse_func_iri` will parse
  not only well-formed data now.

### Fixed
- `Paint::from_span` poor performance.

[Unreleased]: https://github.com/RazrFalcon/svgtypes/compare/v0.2.0...HEAD
[0.2.0]: https://github.com/RazrFalcon/svgtypes/compare/v0.1.1...v0.2.0
[0.1.1]: https://github.com/RazrFalcon/svgtypes/compare/v0.1.0...v0.1.1
