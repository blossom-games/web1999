-- Simple pandoc filter that removes images.
--
-- This is used to build the "plain-text" (actually reformatted markdown)
-- README file for releases.
function Image () return {} end
function Figure () return {} end
