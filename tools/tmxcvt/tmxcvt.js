#!/usr/bin/env node

const { XMLParser, XMLBuilder, XMLValidator} = require("fast-xml-parser");
const fs = require("fs");

if (process.argv.length != 3 && process.argv.length != 4) {
  console.log("usage: tmxcvt.js in-tmx [out-map]");
  process.exit(1);
}

const text = fs.readFileSync(process.argv[2], { encoding: "utf8", flag: "r" });
const xml = new XMLParser({ ignoreAttributes: false }).parse(text);

const map_data = xml.map.layer.data["#text"].split(",").map((x) => parseInt(x).toString(16));

const out = [];

const numObjects = xml.map.objectgroup.object.length;
out.push(numObjects)

for (let i = 0; i < numObjects; i++) {
  const object = xml.map.objectgroup.object[i];
  out.push(object["@_name"]);
  out.push(object["@_x"]);
  out.push(object["@_y"]);
}

const width = parseInt(xml.map.layer["@_width"]);
const height = parseInt(xml.map.layer["@_height"]);
out.push(width);
out.push(height);

for (let y = height - 1; y >= 0; y--) {
  for (let x = 0; x < width; x++) {
    out.push(map_data[y * width + x]);
  }
}

if (process.argv.length == 4) {
  fs.writeFileSync(process.argv[3], out.join(" "));
} else {
  console.log(out.join(" "));
}
