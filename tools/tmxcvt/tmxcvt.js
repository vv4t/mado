#!/usr/bin/env node

const { XMLParser, XMLBuilder, XMLValidator} = require("fast-xml-parser");
const fs = require("fs");

const opt = {
  ignoreAttributes: false
};

const text = fs.readFileSync("saves/arena.tmx", { encoding: "utf8", flag: "r" });
const xml = new XMLParser(opt).parse(text);

const width = parseInt(xml.map.layer["@_width"]);
const height = parseInt(xml.map.layer["@_height"]);

const raw_data = xml.map.layer.data["#text"].split(",").map((x) => parseInt(x).toString(16));

console.log(width, height);

const data = [];

for (let y = height - 1; y >= 0; y--) {
  for (let x = 0; x < width; x++) {
    data.push(raw_data[y * width + x]);
  }
}

console.log(data.join(" "));
