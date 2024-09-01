#!/usr/bin/env node

const { XMLParser, XMLBuilder, XMLValidator } = require("fast-xml-parser");
const fs = require("fs");

if (process.argv.length != 3 && process.argv.length != 4) {
  console.log("usage: tmxcvt.js in-tmx [out-map]");
  process.exit(1);
}

const text = fs.readFileSync(process.argv[2], { encoding: "utf8", flag: "r" });
const xml = new XMLParser({
  ignoreAttributes: false,
  isArray: (name) => [ 'objectgroup', 'object', 'layer' ].includes(name)
}).parse(text);

const layer = xml.map.layer[0];
const map_data = layer.data["#text"].split(",").map((x) => parseInt(x).toString(16));

const width = layer["@_width"];
const height = layer["@_height"];

const out = [];

out.push(xml.map.objectgroup.length);

for (const group of xml.map.objectgroup) {
  out.push(group['@_name']);
  out.push(group.object.length);
  
  for (const object of group.object) {
    out.push(object["@_name"]);
    out.push(object["@_x"] / xml.map['@_tilewidth']);
    out.push(height - object["@_y"] / xml.map['@_tileheight']);
  }
}

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
